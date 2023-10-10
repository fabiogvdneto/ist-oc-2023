CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L2Cache

all:
	$(CC) $(CFLAGS) ../tests/SimpleProgram.c L2Cache.c -o $(TARGET)

test1:
	$(CC) $(CFLAGS) ../tests/test1.c L2Cache.c -o $(TARGET)

run:
	./$(TARGET) > out.txt

clean:
	rm $(TARGET)
	rm out.txt