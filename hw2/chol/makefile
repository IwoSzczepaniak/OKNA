CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O2 -Wall -msse3 -march=znver2 -mavx -lm
LDFLAGS    := -lm

all:
	make run_all
	make draw
	
draw:
	python OwnPlot.py loop	
	python OwnPlot.py vecs
	python OwnPlot.py

run:
	$(CC) $(VERSION).c $(CFLAGS)
	./a.out >> output_$(VERSION).m

run_all:
	./run_all.sh

clean:
	rm -f *.out *.m *.png
