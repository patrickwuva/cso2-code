CC = clang
CFLAGS = -Wall -Wno-unused-variable
LDFLAGS = -lm
SRCS := mypagerisbeeping.c help.c
OBJS := $(SRCS:.c=.o)
DEPs := mltp.h config.h

LIB := libmlpt.a

ifeq ($(d),y)
CFLAGS += -g
endif
ifeq ($(f),y)
CFLAGS += -fsanitize=address
endif
ifeq ($(g),y)
CFLAGS += -g -fsanitize=address
endif
all: $(LIB) testexe
$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
testexe:
	$(CC) $(CFLAGS) test.c libmlpt.a -lm -o test


.PHONY: all clean test

