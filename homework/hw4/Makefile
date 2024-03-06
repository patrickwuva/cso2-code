CC = clang
CFLAGS = -Wall
LDFLAGS = -lm
SRCS := mypagerisbeeping.c
OBJS := $(SRCS:.c=.o)
DEPs := mltp.h config.h

LIB := libmlpt.a

ifeq ($(d),y)
CFLAGS += -g
endif
ifeq ($(f),y)
CFLAGS += -fsanitize=address -fno-omit-frame-pointer -g
endif
all: $(LIB)
$(LIB): $(OBJS)
	ar rcs $@ $(OBJS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
test:
	$(CC) $(CFLAGS) test.c libmlpt.a -lm -o test


.PHONY: all clean test

