#include "../4.1/L1Cache.h"

int main() {
  resetTime();
  initCache();
  unsigned char value;

  // FIRST ROUND: same index but different word offset.

  printf("MODE\tADDR\tVALUE\tTIME\n");

  uint8_t val1 = 11;
  uint8_t val2 = 22;
  uint8_t val3 = 33;
  uint8_t val4 = 44;

  uint32_t addr1 = 0x4;
  uint32_t addr2 = 0x8;
  uint32_t addr3 = 0xC;
  uint32_t addr4 = 0xF;

  write(addr1, &val1);
  printf("write\t%u\t%u\t%d\n", addr1, val1, getTime());
  write(addr2, &val2);
  printf("write\t%u\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, &val3);
  printf("write\t%u\t%u\t%d\n", addr3, val3, getTime());
  write(addr4, &val4);
  printf("write\t%u\t%u\t%d\n", addr4, val4, getTime());

  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr1, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr2, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr3, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr4, value, getTime());

  // SECOND ROUND: same as first round, but different index this time.

  addr1 = 0x104;
  addr2 = 0x108;
  addr3 = 0x10C;
  addr4 = 0x10F;

  write(addr1, &val1);
  printf("write\t%u\t%u\t%d\n", addr1, val1, getTime());
  write(addr2, &val2);
  printf("write\t%u\t%u\t%d\n", addr2, val2, getTime());
  write(addr3, &val3);
  printf("write\t%u\t%u\t%d\n", addr3, val3, getTime());
  write(addr4, &val4);
  printf("write\t%u\t%u\t%d\n", addr4, val4, getTime());

  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr1, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr2, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr3, value, getTime());
  read(addr1, &value);
  printf("read\t%u\t%u\t%d\n", addr4, value, getTime());

  
  return 0;
}