#include "L1Cache.h"

uint8_t DRAM[DRAM_SIZE];
// number of bits for word, tag, index and offset.
uint8_t bits_word;
uint8_t bits_tag;
uint8_t bits_index;
uint8_t bits_offset;
// mask for tag, index and offset.
uint32_t mask_tag;
uint32_t mask_index;
uint32_t mask_offset;
uint32_t time;
Cache L1Cache;

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

  if (!L1Cache.init) {
    for (int i = 0; i < sizeof(lines); i++) {
      lines[i].valid = 0;
    }

    bits_word = log2(WORD_SIZE);
    bits_offset = log2(BLOCK_SIZE);
    bits_index = log2(sizeof(lines));
    bits_tag = log2(ADDRESS_SIZE) - bits_index - bits_offset;

    mask_offset  = (BLOCK_SIZE - 1);
    mask_index = (sizeof(lines) - 1) << bits_offset;
    mask_tag = (ADDRESS_SIZE - 1) ^ (mask_offset) ^ (mask_index);
    L1Cache.init = 1;
  }
  
  uint32_t offset = (parse_offset(address) >> bits_word) * WORD_SIZE;
  uint32_t index = parse_index(address);
  uint32_t tag = filter_tag(address);

  lines += index;

  if (!lines->valid || lines->tag != tag) {
    uint32_t memAddress = (address >> bits_offset) << bits_offset;
    uint8_t tempBlock[BLOCK_SIZE];

    accessDRAM(memAddress, tempBlock, MODE_READ); // get new block from DRAM

    if ((lines->valid) && (lines->dirty)) { // line has dirty block
      accessDRAM(memAddress, lines, MODE_WRITE); // then write back old block
    }

    memcpy(lines, tempBlock, BLOCK_SIZE); // copy new block to cache line
    lines->valid = 1;
    lines->dirty = 0;
    lines->tag = tag;
  } // if miss, then replaced with the correct block

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, lines->data[offset], WORD_SIZE);
    time += L1_READ_TIME;
    return;
  }

  if (mode == MODE_WRITE) { // write data from cache line
    memcpy(lines->data[offset], data, WORD_SIZE);
    time += L1_WRITE_TIME;
    lines->dirty = 1;
    return;
  }
}

int filter_tag(uint32_t address) {
  return (address & mask_tag);
}

int parse_index(uint32_t address) {
  return (address & mask_index) >> bits_offset;
}

int parse_offset(uint32_t address) {
  return (address & mask_offset);
}

void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}
