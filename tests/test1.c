#include "../4.1/L1Cache.h"

int main() {

  // set seed for random number generator
  srand(0);

  resetTime();
  initCache();
  int value;

  int a = 8;
  write(a, (unsigned char *) (&a));
  printf("WRITE %d IN ADDRESS %d; TIME: %d\n", a, a, getTime());

  read(a, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", a, value, getTime());

  int b = 16384;
  write(b, (unsigned char *) (&b));
  printf("WRITE %d IN ADDRESS %d; TIME: %d\n", b, b, getTime());

  read(b, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", b, value, getTime());

  read(a, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", a, value, getTime());

  read(b, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", b, value, getTime());
  
  return 0;
}