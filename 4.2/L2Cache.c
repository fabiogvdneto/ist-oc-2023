#include "L2Cache.h"

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

void initCache() {
  cacheL1.init = 0;
  cacheL2.init = 0;
}

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address / BLOCK_SIZE) % (L2_LINE_COUNT);
  uint32_t tag = (address / L2_SIZE);

  CacheLine* line = cacheL2.lines + index;

  // Make sure data block is present in cache L2. If not, fetch block from RAM.
  if (!line->valid || line->tag != tag) {
    uint32_t memAddress = (address / BLOCK_SIZE) * BLOCK_SIZE;
    uint8_t tempBlock[BLOCK_SIZE];

    accessDRAM(memAddress, tempBlock, MODE_READ);

    if ((line->valid) && (line->dirty)) {
      memAddress = (line->tag * L2_SIZE) | (index * BLOCK_SIZE);
      accessDRAM(memAddress, line->data, MODE_WRITE);
    }

    memcpy(line->data, tempBlock, BLOCK_SIZE);
    line->valid = 1;
    line->dirty = 0;
    line->tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, &line->data[offset], WORD_SIZE);
    time += L2_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy(&line->data[offset], data, WORD_SIZE);
    time += L2_WRITE_TIME;
    line->dirty = 1;
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
  uint32_t tag = (address / (L1_SIZE));

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