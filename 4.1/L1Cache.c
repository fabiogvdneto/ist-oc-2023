#include "L1Cache.h"

Cache L1Cache;
uint8_t DRAM[DRAM_SIZE];
uint32_t time;

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

void initCache() { L1Cache.init = 0; }

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
  CacheLine* lines = L1Cache.lines;

  // Make sure cache is already initialized. If not, initialize first.
  if (!L1Cache.init) {
    for (int i = 0; i < L1_SIZE/BLOCK_SIZE; i++) {
      lines[i].valid = 0;
    }

    L1Cache.init = 1;
  }
  
  uint32_t offset = (address % BLOCK_SIZE);
  uint32_t index = (address >> L1_OFFSET_BITS) % L1_LINE_COUNT;
  uint32_t tag = (address >> (L1_INDEX_BITS + L1_OFFSET_BITS));

  lines += index;
  
  if (0) {
    printf("%d, %d, %d\n", offset, index, tag);
  }

  // Make sure data block is present in cache. If not, fetch block from RAM.
  if (!lines->valid || lines->tag != tag) {
    uint32_t memAddress = (address >> L1_OFFSET_BITS) << L1_OFFSET_BITS;
    uint8_t tempBlock[BLOCK_SIZE];

    accessDRAM(memAddress, tempBlock, MODE_READ);

    if ((lines->valid) && (lines->dirty)) {
      accessDRAM(memAddress, lines->data, MODE_WRITE);
    }

    memcpy(lines->data, tempBlock, BLOCK_SIZE);
    lines->valid = 1;
    lines->dirty = 0;
    lines->tag = tag;
  }

  if (mode == MODE_READ) {
    memcpy(data, &lines->data[offset], WORD_SIZE);
    time += L1_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) {
    memcpy(&lines->data[offset], data, WORD_SIZE);
    time += L1_WRITE_TIME;
    lines->dirty = 1;
    return;
  }
}
