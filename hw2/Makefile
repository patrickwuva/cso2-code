CC = clang
CFLAGS = -O1

gettimings: main.o functions.o signofthetimes.o
	$(CC) -o $@ $^

%.o: %.c gettimings.h
	$(CC) $(CFLAGS) -o $@ -c $<

functions.o: functions.c gettimings.h
	$(CC) -O0 -o $@ -c $<

clean:
	rm -f *.o gettimings

.PHONY: clean
