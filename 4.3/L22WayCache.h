#ifndef _L2CACHE_H_
#define _L2CACHE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../Cache.h"

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL2(uint32_t, uint8_t *, uint32_t);
void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct {
  uint8_t data[BLOCK_SIZE];
  uint8_t dirty;
  uint32_t valid;
  uint32_t tag;
} CacheLine;

typedef struct {
  CacheLine lines[L1_LINE_COUNT];
} CacheL1;

typedef struct {
  CacheLine lines[L2_LINE_COUNT];
} CacheL2;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif
