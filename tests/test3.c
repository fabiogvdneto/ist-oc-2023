#include "../4.3/L22WayCache.h"

int main() {

  // set seed for random number generator
  srand(0);

  resetTime();
  initCache();
  int value;

  int aaddr = 0;
  int baddr = 1<<14;
  int a = 10;
  int b = 20;

  write(aaddr, (unsigned char *) (&a));
  printf("WRITE %d IN ADDRESS %d; TIME: %d\n", a, aaddr, getTime());

  write(baddr, (unsigned char *) (&b));
  printf("WRITE %d IN ADDRESS %d; TIME: %d\n", b, baddr, getTime());

  read(aaddr, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", aaddr, value, getTime());

  read(baddr, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", baddr, value, getTime());

  read(aaddr, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", aaddr, value, getTime());

  read(baddr, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", baddr, value, getTime());

  int c = 30;

  write(0x3, (unsigned char *) &c);
  write(0x1, (unsigned char *) &b);

  read(0x1, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 0x1 VALUE: %d; TIME: %d\n", value, getTime());

  printL1();
  printL2();
  
  return 0;
}