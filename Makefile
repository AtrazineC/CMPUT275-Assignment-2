CC = g++
CFLAGS = -c -Wall -O2
LFLAGS = -static

PROGS = graph_concepts
OBJS = graph_concepts.o digraph.o

.PHONY: clean

graph_concepts: graph_concepts.o digraph.o
	$(CC) graph_concepts.o digraph.o -o graph_concepts $(LFLAGS)

graph_concepts.o: graph_concepts.cpp digraph.h
	$(CC) graph_concepts.cpp -o graph_concepts.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm $(PROGS) $(OBJS)
