CC=gcc
CFLAGS=-Wall -g -lm -std=c11
LIBS= graph.o set.o BSTree.o queue.o invertedFunc.o pagerankFunc.o readData.o tfidf.o -lm

searchPagerank: searchPagerank.o $(LIBS)
	$(CC) -o searchPagerank searchPagerank.o $(LIBS)

pagerank: pagerank.o $(LIBS)
	$(CC) -o pagerank pagerank.o $(LIBS)

inverted: inverted.o $(LIBS)
	$(CC) -o inverted inverted.o $(LIBS)

searchTfIdf: searchTfIdf.o $(LIBS)
	$(CC) -o searchTfIdf searchTfIdf.o $(LIBS)

scaled: scaledFootrule.o $(LIBS)
	$(CC) -o scaledFootrule scaledFootrule.o $(LIBS)

clean: 
	rm -f *.o

graph.o : graph.c graph.h
set.o : set.c set.h
BSTree.o : BSTree.c BSTree.h
queue.o : queue.c queue.h
invertedFunc.o : invertedFunc.c invertedFunc.h
pagerankFunc.o : pagerankFunc.c pagerankFunc.h
readData.o :  readData.c readData.h
tfidf.o : tfidf.c tfidf.h
