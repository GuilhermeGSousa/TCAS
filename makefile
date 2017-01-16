CC=g++
CFLAGS = -c
CLINK = -o

all: main.o Broadcaster.o
	$(CC) main.o Broadcaster.o $(CLINK) main

Broadcaster.o: Broadcaster.cpp
	$(CC) $(CFLAGS)  Broadcaster.cpp
main.o: main.cpp
	$(CC) $(CFLAGS)  main.cpp


