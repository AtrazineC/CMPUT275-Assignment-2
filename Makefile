##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::
    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##:
##::
    ##::    By:                 Benjamin Kong -- 1573684
##::                            Lora Ma -------- 1570935
    ##::
##::        CMPUT 275:          Winter 2020
    ##::    Assignment 2:       Part 1
##::
    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##:
##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::    ##::



CC = g++
CFLAGS = -c -Wall -O2
LFLAGS = -static

PROGS = server
OBJS = server.o digraph.o dijkstra.o 

.PHONY: clean

server: server.o digraph.o dijkstra.o
	$(CC) server.o  digraph.o dijkstra.o -o server $(LFLAGS)

server.o: server.cpp
	$(CC) -c server.cpp -o server.o $(CFLAGS)

digraph.o: digraph.cpp
	$(CC) -c digraph.cpp -o digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp dijkstra.h
	$(CC) -c dijkstra.cpp -o dijkstra.o $(CFLAGS)

clean:
	@rm -f $(PROGS) $(OBJS)
