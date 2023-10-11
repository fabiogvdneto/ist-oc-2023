#include "../4.3/L22WayCache.h"

int main() {
  printf("MODE\tADDR\tVALUE\tTIME\n");
  resetTime();
  initCache();

  // FIRST ROUND: same index but different word offset.

  uint32_t value;

  uint32_t val1 = 11;
  uint32_t val2 = 22;
  uint32_t val3 = 33;

  uint32_t addr1 = 0x104;
  uint32_t addr2 = 0x108;
  uint32_t addr3 = 0x10C;

  uint8_t* pvalue = (uint8_t*) &value;
  uint8_t* pval1 = (uint8_t*) &val1;
  uint8_t* pval2 = (uint8_t*) &val2;
  uint8_t* pval3 = (uint8_t*) &val3;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime()); // 111
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime()); // 112
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime()); // 113

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime()); // 114
  read(addr2, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime()); // 115
  read(addr3, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime()); // 116

  // SECOND ROUND: same as first round, but different index this time.

  addr1 = 0x204;
  addr2 = 0x208;
  addr3 = 0x20C;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime()); // 227
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime()); // 228
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime()); // 229

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime()); // 230
  read(addr2, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime()); // 231
  read(addr3, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime()); // 232

  printf("\nMODE\tADDR\tVALUE\tTIME\n");
  resetTime();

  // THIRD ROUND: conflicts (same index, different tag).

  addr1 = 0x4000;
  addr2 = 0x8000;
  addr3 = 0xC000;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime()); // 313
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime());

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime());
  read(addr2, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime());
  read(addr3, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime());
  read(addr2, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime());

  return 0;
}