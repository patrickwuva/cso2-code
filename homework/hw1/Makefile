CC=clang
#CFLAGS=-fsanitize=address -ftrivial-auto-var-init=pattern
CFLAGS=-ftrivial-auto-var-init=pattern
#CFLAGS=-Wall
split: split.o main.o
	$(CC) $(CFLAGS) split.o main.o -o $@

%.o: %.c split.c
	$(CC) $(CFLAGS) -o $@ -c $<
