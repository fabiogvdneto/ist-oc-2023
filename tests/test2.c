#include "../4.2/L2Cache.h"

int main() {

  // set seed for random number generator
  srand(0);

  resetTime();
  initCache();
  int value;

  int a = 0;
  write(a, (unsigned char *) (&a));
  printf("WRITE 0 IN ADDRESS 0; TIME: %d\n", getTime());

  read(a, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  int b = 32768;
  write(b, (unsigned char *) (&b));
  printf("WRITE 32768 IN ADDRESS 32768; TIME: %d\n", getTime());

  read(b, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 32768 VALUE: %d; TIME: %d\n", value, getTime());

  read(a, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  read(b, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 32768 VALUE: %d; TIME: %d\n", value, getTime());
  
  return 0;
}