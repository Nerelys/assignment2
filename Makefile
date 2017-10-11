CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=graph.o queue.o set.o	pagerank.o

search: search.o $(LIBS)
	$(CC) -o search search.o $(LIBS)

pagerank : pagerank.c pagerank.h
graph.o : graph.c graph.h
queue.o : queue.c queue.h
set.o	  : set.c set.h
