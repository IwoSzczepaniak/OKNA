CC=gcc
# CFLAGS = -lm -mavx
CFLAGS = -lm -mavx -O2 -march=native -mfma

SRCS=$(wildcard *.c)
BINS=$(SRCS:.c=)

.PHONY: all clean run

all: $(BINS)

%: %.c
	$(CC) $< -o $@ $(CFLAGS) 

run: $(BINS)
	@for bin in $(BINS); do \
		./$$bin 4000; \
	done

clean:
	rm -f $(BINS)
