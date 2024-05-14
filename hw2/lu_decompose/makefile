CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O2 -Wall -msse3 -march=znver2
LDFLAGS    := -lm

all:
	make run_all
	python OwnPlot.py	

run:
	$(CC) $(CFLAGS) $(VERSION).c
	./a.out >> output_$(VERSION).m

run_all:
	./run_all.sh

clean:
	rm -f *.out *.m *.png
