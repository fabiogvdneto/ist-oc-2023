CC = gcc
CFLAGS=-Wall -Wextra

all:
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.1/L1Cache.c -o tests/L1Cache
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.2/L2Cache.c -o tests/L2Cache
	$(CC) $(CFLAGS) tests/SimpleProgram.c 4.3/L22WayCache.c -o tests/L22WayCache


run:
	./tests/L1Cache > tests/4.1.out
	./tests/L2Cache > tests/4.2.out
	./tests/L22WayCache > tests/4.3.out

diff1:
	diff tests/4.1.out tests/results_L1.txt

diff2:
	diff tests/4.2.out tests/results_L2_1W.txt

diff3:
	diff tests/4.3.out tests/results_L2_2W.txt

clean:
	rm tests/L1Cache
	rm tests/L2Cache
	rm tests/L22WayCache
	rm tests/*.out