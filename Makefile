CC = gcc
CFLAGS=-Wall -Wextra

all:
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.1/L1Cache.c -o L1Cache
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.2/L2Cache.c -o L2Cache
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.3/L22WayCache.c -o L22WayCache


run:
	./L1Cache > 4.1.out
	./L2Cache > 4.2.out
	./L22WayCache > 4.3.out


clean:
	rm $(TARGET)
	rm *.out