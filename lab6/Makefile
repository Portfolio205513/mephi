CC=gcc
CFLAGS=-Wall -std=c99 -g

all: lab6

lab6: main.o queue.o 
	$(CC) main.o queue.o -o lab6

main.o: main.c
	$(CC) $(CFLAGS) main.c -c

queue.o: queue.c
	$(CC) $(CFLAGS) queue.c -c

clean:
	rm -rf *.o lab6