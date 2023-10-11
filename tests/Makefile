CC = gcc
CFLAGS=-Wall -Wextra
TARGET=exec

all:
	$(CC) $(CFLAGS) SimpleProgram.c ../4.2/L2Cache.c -o $(TARGET)

test2:
	$(CC) $(CFLAGS) test2.c ../4.2/L2Cache.c -o $(TARGET)

clean:
	rm $(TARGET)
	rm *.out