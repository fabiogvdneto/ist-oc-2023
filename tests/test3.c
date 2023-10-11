#include "../4.3/L22WayCache.h"

void writef(uint32_t address, uint32_t* value) {
  write(address, (uint8_t*) value);
  printf("write\t%d\t%u\t%d\n", address, *value, getTime());
}

void readf(uint32_t address, uint32_t* value) {
  read(address, (uint8_t*) value);
  printf("read\t%d\t%u\t%d\n", address, *value, getTime());
}

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

  writef(addr1, &val1); // 111
  writef(addr2, &val2); // 112
  writef(addr3, &val3); // 113

  readf(addr1, &value); // 114
  readf(addr2, &value); // 115
  readf(addr3, &value); // 116

  // SECOND ROUND: same as first round, but different index this time.

  addr1 = 0x204;
  addr2 = 0x208;
  addr3 = 0x20C;

  writef(addr1, &val1); // 227
  writef(addr2, &val2); // 228
  writef(addr3, &val3); // 229

  readf(addr1, &value); // 230
  readf(addr2, &value); // 231
  readf(addr3, &value); // 232

  printf("\nMODE\tADDR\tVALUE\tTIME\n");
  resetTime();

  // THIRD ROUND: conflicts (same index, different tag).

  addr1 = 0x4000;
  addr2 = 0x8000;
  addr3 = 0xC000;

  writef(addr1, &val1); // 313
  writef(addr2, &val2);
  readf(addr1, &value);
  writef(addr3, &val3);

  readf(addr1, &value);
  readf(addr3, &value);
  readf(addr2, &value);
  readf(addr1, &value);

  return 0;
}