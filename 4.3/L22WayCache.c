#include "L22WayCache.h"

Cache cacheL1;
Cache cacheL2;
uint8_t DRAM[DRAM_SIZE];
uint32_t time;


/* ---- Helper ---- */

void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}


/* ---- RAM ---- */

void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

  if (address >= DRAM_SIZE - WORD_SIZE + 1)
    exit(-1);

  if (mode == MODE_READ) {
    memcpy(data, (DRAM+address), BLOCK_SIZE);
    time += DRAM_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy((DRAM+address), data, BLOCK_SIZE);
    time += DRAM_WRITE_TIME;
    return;
  }
}


/* ---- Cache Hierarchy ---- */

uint8_t lru(CacheLine* line) {
  return (line->valid < (line+1)->valid) ? 0 : 1;
}

int8_t find(CacheLine* set, uint32_t tag) {
  if (set[0].valid && set[0].tag == tag) {
    if (set[1].valid) {
      set[1].valid = 1;
    }

    set[0].valid = 2;
    return 0;
  }

  if (set[1].valid && set[1].tag == tag) {
    if (set[0].valid) {
      set[0].valid = 1;
    }

    set[1].valid = 2;
    return 1;
  }

  return -1;
}

void initCache() {
  cacheL1.init = 0;
  cacheL2.init = 0;
}

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address / BLOCK_SIZE) % (L2_LINE_COUNT/2);
  uint32_t tag = (address / (BLOCK_SIZE * L2_LINE_COUNT/2));

  CacheLine* set = cacheL2.lines + index * 2;

  int8_t way = find(set, tag);

  // Make sure data block is present in cache L2. If not, fetch block from RAM.
  if (way == -1) {
    way = lru(set);

    uint32_t memAddress = (address / BLOCK_SIZE) * BLOCK_SIZE;
    uint8_t tempBlock[BLOCK_SIZE];

    accessDRAM(memAddress, tempBlock, MODE_READ);

    if ((set[way].valid) && (set[way].dirty)) {
      memAddress = (set[way].tag * BLOCK_SIZE * L2_LINE_COUNT/2) | (index * BLOCK_SIZE);
      accessDRAM(memAddress, set[way].data, MODE_WRITE);
    }

    memcpy(set[way].data, tempBlock, BLOCK_SIZE);
    set[way].valid = 1;
    set[way].dirty = 0;
    set[way].tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, (set[way].data+offset), WORD_SIZE);
    time += L2_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy((set[way].data+offset), data, WORD_SIZE);
    time += L2_WRITE_TIME;
    set[way].dirty = 1;
    return;
  }
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
  // Make sure caches are already initialized. If not, initialize first.
  if (!cacheL1.init) {
    cacheL1.lines = (CacheLine*) calloc(L1_LINE_COUNT, sizeof(CacheLine));
    cacheL2.lines = (CacheLine*) calloc(L2_LINE_COUNT, sizeof(CacheLine));

    for (int i = 0; i < L1_LINE_COUNT; i++) {
      cacheL1.lines[i].valid = 0;
      cacheL2.lines[i].valid = 0;
    }

    for (int i = L1_LINE_COUNT; i < L2_LINE_COUNT; i++) {
      cacheL2.lines[i].valid = 0;
    }
    
    cacheL1.init = 1;
    cacheL2.init = 1;
  }
  
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address / BLOCK_SIZE) % (L1_LINE_COUNT);
  uint32_t tag = (address / (BLOCK_SIZE * L1_LINE_COUNT));

  CacheLine* line = cacheL1.lines + index;

  // Make sure data block is present in cache L1. If not, fetch block from Cache L2.
  if (!line->valid || line->tag != tag) {
    uint8_t tempBlock[BLOCK_SIZE];

    accessL2(address, tempBlock, MODE_READ);

    if ((line->valid) && (line->dirty)) {
      accessL2(address, line->data, MODE_WRITE);
    }

    memcpy(line->data, tempBlock, BLOCK_SIZE);
    line->valid = 1;
    line->dirty = 0;
    line->tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, &line->data[offset], WORD_SIZE);
    time += L1_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy(&line->data[offset], data, WORD_SIZE);
    time += L1_WRITE_TIME;
    line->dirty = 1;
    return;
  }
}


/* ---- Timer ---- */

void resetTime() { time = 0; }

uint32_t getTime() { return time; }