CC = gcc
CFLAGS=-Wall -Wextra
TARGET=L22WayCache

all:
	$(CC) $(CFLAGS) ../tests/SimpleProgram.c L22WayCache.c -o $(TARGET)

test1:
	$(CC) $(CFLAGS) ../tests/test1.c L22WayCache.c -o $(TARGET)

run:
	./$(TARGET) > out.txt

clean:
	rm $(TARGET)
	rm out.txt