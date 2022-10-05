CC=gcc
CFLAGS=-Wall -std=c99 -g

all: lab4

lab4: main.o part.o sorts.o 
	$(CC) main.o part.o sorts.o -o lab4 -lm

main.o: lab4.c
	$(CC) $(CFLAGS) lab4.c -c -o main.o

part.o: part.c
	$(CC) $(CFLAGS) part.c -c

sotrs.o: sorts.c
	$(CC) $(CFLAGS) sorts.c -c

clean:
	rm -rf *.o lab4
