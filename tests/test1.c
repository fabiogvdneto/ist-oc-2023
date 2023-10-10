#include "../4.1/L1Cache.h"

int main() {

  // set seed for random number generator
  srand(0);

  resetTime();
  initCache();
  int value;

  int a = 0;
  write(a, (unsigned char *) (&a));
  printf("1. WRITE 0 IN ADDRESS 0; TIME: %d\n", getTime());

  read(a, (unsigned char *) (&value));
  printf("2. READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  int c = 4;
  int d = 1000;
  write (c, (unsigned char *) (&d));
  printf("3. WRITE 1000 IN ADDRESS 4; TIME: %d\n", getTime());

  read(c, (unsigned char *) (&value));
  printf("6. READ FROM ADDRESS 4 VALUE: %d; TIME: %d\n", value, getTime());

  int b = 16384;
  write(b, (unsigned char *) (&b));
  printf("3. WRITE 16384 IN ADDRESS 16384; TIME: %d\n", getTime());

  read(b, (unsigned char *) (&value));
  printf("4. READ FROM ADDRESS 16384 VALUE: %d; TIME: %d\n", value, getTime());

  read(a, (unsigned char *) (&value));
  printf("5. READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  read(b, (unsigned char *) (&value));
  printf("6. READ FROM ADDRESS 16384 VALUE: %d; TIME: %d\n", value, getTime());
  
  return 0;
}