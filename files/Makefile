CFLAGS=-std=c99 -w

all: clean test graph matrix task

test: test.o
	gcc -o $@ -pthread test.o

test.o:
	gcc -o $@ -c test.c $(CFLAGS)

graph:
	gcc -o $@ $(CFLAGS) $@.c

matrix:
	gcc -o $@ $(CFLAGS) $@.c

task:
	gcc -o $@ $(CFLAGS) $@.c

clean:
	rm -f test.o test graph matrix task
