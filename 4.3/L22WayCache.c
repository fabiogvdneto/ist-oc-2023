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

void initCache() {
  cacheL1.init = 0;
  cacheL2.init = 0;
}

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address / BLOCK_SIZE) % (L2_LINE_COUNT/L2_ASSOCIATIVITY);
  uint32_t tag = (address / (L2_SIZE/L2_ASSOCIATIVITY));

  CacheLine* set = cacheL2.lines + index * L2_ASSOCIATIVITY;
  int8_t iset = L2_ASSOCIATIVITY;

  // Find block inside the associativity set.
  while (--iset >= 0) {
    if (set[iset].valid && set[iset].tag == tag) break;
  }

  // If block was not found, fetch from RAM.
  if (iset == -1) {
    uint8_t tempBlock[BLOCK_SIZE];

    // Find least recently used (LRU) block in the associativity set.
    for (int i = 1, iset = 0; i < L2_ASSOCIATIVITY; i++) {
      if (set[i].valid < set[iset].valid) {
        iset = i;
      }
    }

    accessDRAM(address-offset, tempBlock, MODE_READ);

    if (set[iset].valid && set[iset].dirty) {
      address = (set[iset].tag * L2_SIZE/L2_ASSOCIATIVITY) + (index * BLOCK_SIZE);
      accessDRAM(address, set[iset].data, MODE_WRITE);
    }

    memcpy(set[iset].data, tempBlock, BLOCK_SIZE);
    set[iset].valid = time;
    set[iset].dirty = 0;
    set[iset].tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, set[iset].data, BLOCK_SIZE);
    time += L2_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy(set[iset].data, data, BLOCK_SIZE);
    time += L2_WRITE_TIME;
    set[iset].dirty = 1;
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
  uint32_t index = (address / BLOCK_SIZE) % L1_LINE_COUNT;
  uint32_t tag = (address / L1_SIZE);

  CacheLine* line = cacheL1.lines + index;

  // Make sure data block is present in cache L1. If not, fetch block from Cache L2.
  if (!line->valid || line->tag != tag) {
    uint8_t tempBlock[BLOCK_SIZE];

    accessL2(address, tempBlock, MODE_READ);
    
    if (line->valid && line->dirty) {
      accessL2((line->tag * L1_SIZE) + (index * BLOCK_SIZE), line->data, MODE_WRITE);
    }

    memcpy(line->data, tempBlock, BLOCK_SIZE);
    line->valid = 1;
    line->dirty = 0;
    line->tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, (line->data+offset), WORD_SIZE);
    time += L1_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy((line->data+offset), data, WORD_SIZE);
    time += L1_WRITE_TIME;
    line->dirty = 1;
    return;
  }
}


/* ---- Timer ---- */

void resetTime() { time = 0; }

uint32_t getTime() { return time; }


/* ---- Debugging ---- */

void printL1() {
  CacheLine* lines = cacheL1.lines;

  printf("L1\n");

  for (int i = 0; i < L1_LINE_COUNT; i++) {
    if (lines[i].valid) {
      printf("Addre: %d\n", (lines[i].tag * L1_SIZE + i * BLOCK_SIZE));
      printf("Value: %u\n", (unsigned int) *lines[i].data);
    }
  }
}

void printL2() {
  CacheLine* lines = cacheL2.lines;

  printf("L2\n");

  for (int i = 0; i < L2_LINE_COUNT; i++) {
    if (lines[i].valid) {
      printf("Addre: %d\n", (lines[i].tag * L2_SIZE + i * BLOCK_SIZE/L2_ASSOCIATIVITY));
      printf("Value: %u\n", (unsigned int) *lines[i].data);
    }
  }
}