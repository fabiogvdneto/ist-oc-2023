#include "L1Cache.h"

Cache cache;
uint8_t DRAM[DRAM_SIZE];
uint32_t time = 0;
uint32_t init = 0;

/**************** Helper functions ****************/

void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
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

/*********************** L1 cache *************************/

void initCache() {
  for (int i = 0; i < L1_LINE_COUNT; i++) {
    cache.lines[i].valid = 0;
  }

  init = 1;
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
  CacheLine* lines = cache.lines;
  
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address / BLOCK_SIZE) % (L1_LINE_COUNT);
  uint32_t tag = (address / (L1_SIZE));

  lines += index;

  // Make sure data block is present in cache. If not, fetch block from RAM.
  if (!lines->valid || lines->tag != tag) {
    uint8_t tempBlock[BLOCK_SIZE];

    accessDRAM((address-offset), tempBlock, MODE_READ);

    if ((lines->valid) && (lines->dirty)) {
      accessDRAM((lines->tag * L1_SIZE) + (index * BLOCK_SIZE), lines->data, MODE_WRITE);
    }

    memcpy(lines->data, tempBlock, BLOCK_SIZE);
    lines->valid = 1;
    lines->dirty = 0;
    lines->tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, (lines->data+offset), WORD_SIZE);
    time += L1_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy((lines->data+offset), data, WORD_SIZE);
    time += L1_WRITE_TIME;
    lines->dirty = 1;
    return;
  }
}