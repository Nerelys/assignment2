// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetRep *Set;

// Function signatures

int nodeVal(Set, char *);
float retrieveRank(Set, int);
char * retrieveVal(Set, int);
Set newSet();
void disposeSet(Set);
void insertInto(Set, char *, float);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);
void showSet(Set);

#endif
