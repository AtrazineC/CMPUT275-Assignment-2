##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::
    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##:
##::
    ##::    By:                 Benjamin Kong -- 1573684
##::                            Lora Ma -------- 1570935
    ##::
##::        CMPUT 275:          Winter 2020
    ##::    Assignment 2:       Part 2
##::
    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##:
##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::

CC = g++
CFLAGS = -c -Wall -O2
LFLAGS = -static

PROGS = server
OBJS = server.o digraph.o dijkstra.o serialport.o

.PHONY: clean

server: server.o digraph.o dijkstra.o serialport.o
	$(CC) server.o digraph.o dijkstra.o serialport.o -o server $(LFLAGS)

server.o: server.cpp
	$(CC) -c server.cpp -o server.o $(CFLAGS)

dijkstra.o: dijkstra.h dijkstra.cpp heap.h digraph.h wdigraph.h
	$(CC) -c dijkstra.cpp -o dijkstra.o $(CFLAGS)

digraph.o: digraph.h digraph.cpp
	$(CC) -c digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm -f $(PROGS) $(OBJS)
