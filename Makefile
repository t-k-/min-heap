.PHONY: all clean

CC=gcc

all: test.out

test.out: main.o minheap.o
	gcc $^ -o $@

clean:
	rm -f *.o *.out tags
