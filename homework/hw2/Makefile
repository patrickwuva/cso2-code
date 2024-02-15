CC = clang
CFLAGS = -O1

gettimings: main.o signofthetimes.o helpers.c
	$(CC) -o $@ $^

%.o: %.c gettimings.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o gettimings

.PHONY: clean
