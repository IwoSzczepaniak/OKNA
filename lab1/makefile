CC=gcc
CFLAGS=-std=c11

SRCS=$(wildcard *.c)
BINS=$(SRCS:.c=)

.PHONY: all clean

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)
