#include "../4.1/L1Cache.h"

int main() {
  resetTime();
  initCache();

  printf("MODE\tADDR\tVALUE\tTIME\n");

  // FIRST ROUND: same index but different word offset.

  uint32_t value;

  uint32_t val1 = 11;
  uint32_t val2 = 22;
  uint32_t val3 = 33;
  uint32_t val4 = 44;

  uint8_t* pvalue = (uint8_t*) &value;

  uint8_t* pval1 = (uint8_t*) &val1;
  uint8_t* pval2 = (uint8_t*) &val2;
  uint8_t* pval3 = (uint8_t*) &val3;
  uint8_t* pval4 = (uint8_t*) &val4;

  uint32_t addr1 = 0x4;
  uint32_t addr2 = 0x8;
  uint32_t addr3 = 0xC;
  uint32_t addr4;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime());
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime());

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime());

  // SECOND ROUND: same as first round, but different index this time.

  addr1 = 0x104;
  addr2 = 0x108;
  addr3 = 0x10C;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime());
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime());

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime());

  // THIRD ROUND: conflicts (same index, different tag).

  addr1 = 0x10000;
  addr2 = 0x20000;
  addr3 = 0x30000;
  addr4 = 0x40000;

  write(addr1, pval1);
  printf("write\t%d\t%u\t%d\n", addr1, val1, getTime());
  write(addr2, pval2);
  printf("write\t%d\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, pval3);
  printf("write\t%d\t%u\t%d\n", addr3, val3, getTime());
  write(addr4, pval4);
  printf("write\t%d\t%u\t%d\n", addr4, val4, getTime());

  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr1, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr2, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr3, value, getTime());
  read(addr1, pvalue);
  printf("read\t%d\t%u\t%d\n", addr4, value, getTime());

  
  return 0;
}