#include "../4.3/L22WayCache.h"

int main() {

  // set seed for random number generator
  srand(0);

  resetTime();
  initCache();
  int value;
  int a = 10;
  int b = 20;

  write(0, (unsigned char *) (&a));
  printf("WRITE %d IN ADDRESS 0; TIME: %d\n", a, getTime());

  write(1<<14, (unsigned char *) (&b));
  printf("WRITE %d IN ADDRESS %d; TIME: %d\n", b, 1 << 14, getTime());

  read(0, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  read(1<<14, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", 1<<14, value, getTime());

  read(0, (unsigned char *) (&value));
  printf("READ FROM ADDRESS 0 VALUE: %d; TIME: %d\n", value, getTime());

  read(1<<14, (unsigned char *) (&value));
  printf("READ FROM ADDRESS %d VALUE: %d; TIME: %d\n", 1<<14, value, getTime());
  
  return 0;
}