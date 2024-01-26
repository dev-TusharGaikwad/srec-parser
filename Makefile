CC=gcc
CFLAGS=-I .
LIB=./lib

all: 
	$(CC) $(CFLAGS) main.cpp -o srec.o

clean:
	rm -f srec.o

run: all
	./srec.o