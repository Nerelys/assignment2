CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=BSTree.o graph.o queue.o set.o	pagerank.o readData.o inverted.o tfidf.o -lm

search: searchTfIdf.o $(LIBS)
	$(CC) -o searchTfIdf searchTfIdf.o $(LIBS)

tfidf.o : tfidf.c tfidf.h
inverted.o : inverted.c inverted.h
BSTree.o : BSTree.c BSTree.h
readData.o : readData.c readData.h
pagerank.o : pagerank.c pagerank.h
graph.o : graph.c graph.h
queue.o : queue.c queue.h
set.o	  : set.c set.h
