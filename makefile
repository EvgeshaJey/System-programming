CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = process_example

all: $(TARGET)

$(TARGET): main.o
	$(CC) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) *.o
