CC = clang
CFLAGS=-Wall

all: split.o main.o:
	$(CC) $(CFLAGS) split.o main.o -o split

%.o: %.c split.h
	$(CC) $(CFLAGS) -o $@ -c $<
