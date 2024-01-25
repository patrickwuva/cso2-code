#myself upm8pb
CC = clang
CFLAGS = -Wall
LDFLAGS = -Wall
LDLIBS = -L. -lsay

guesser: guesser.o libsay.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c say.h
	$(CC) $(CFLAGS) -o $@ -c $<

libsay.a: cheer.o grunt.o
	ar -rcs libsay.a cheer.o grunt.o

all: libsay.a guesser

clean:
	rm -f *.o *.a guesser

.PHONY: all clean
