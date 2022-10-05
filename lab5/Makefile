CC=gcc
CFLAGS=-Wall -std=c99 -g

all: lab5

lab5: main.o matrix.o streams.o 
	$(CC) main.o matrix.o streams.o -o lab5 -lm

main.o: main.c
	$(CC) $(CFLAGS) main.c -c -o main.o

matrix.o: matrix.c
	$(CC) $(CFLAGS) matrix.c -c

streams.o: streams.c
	$(CC) $(CFLAGS) streams.c -c

clean:
	rm -rf *.o lab5