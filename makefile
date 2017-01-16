CC=g++
CFLAGS = -c
CLINK = -o

all: sender.o Broadcaster.o
	$(CC) sender.o Broadcaster.o $(CLINK) sender
	$(CC) receiver.o Broadcaster.o $(CLINK) receiver

Broadcaster.o: Broadcaster.cpp
	$(CC) $(CFLAGS)  Broadcaster.cpp
sender.o: sender.cpp
	$(CC) $(CFLAGS)  sender.cpp
receiver.o: receiver.cpp
	$(CC) $(CFLAGS)  receiver.cpp

