CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L1Cache

all:
	$(CC) $(CFLAGS) ../tests/SimpleProgram.c L1Cache.c -o $(TARGET)

test1:
	$(CC) $(CFLAGS) ../tests/test1.c L1Cache.c -o $(TARGET)

run:
	./$(TARGET) > out.txt

clean:
	rm $(TARGET)
	rm out.txt