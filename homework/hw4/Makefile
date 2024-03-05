CC = clang
CFLAGS = -Wall
LDFLAGS = -lm
SRCS := mypagerisbeeping.c
OBJS := $(SRCS:.c=.o)
DEPs := mltp.h config.h

LIB := libmlpt.a

all: $(LIB) test

$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)

test:
	$(CC) test.c libmlpt.a -o test
.PHONY: all clean test

