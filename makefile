TARGET = main

CC = gcc
CFLAGS = -Wall -Wextra

ASM = nasm
ASMFLAGS = -f elf64

OBJS = main.o heap.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -no-pie -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

heap.o: heap.s
	$(ASM) $(ASMFLAGS) heap.s -o heap.o

clean:
	rm -f $(TARGET) $(OBJS)