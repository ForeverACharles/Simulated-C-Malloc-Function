CFLAGS = -Wall -std=c99

all: memgrind.c mymalloc.o
	gcc -std=c99 -o memgrind memgrind.c -lm

mymalloc.o: mymalloc.c mymalloc.h
	gcc $(CFLAGS) -c mymalloc.c

clean:
	rm memgrind mymalloc.o

