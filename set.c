// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "set.h"

#define strEQ(s,t) (strcmp((s),(t)) == 0)
#define strLT(s,t) (strcmp((s),(t)) < 0)

typedef struct Node *Link;

typedef struct Node {
	char *val;
	float rank;
	Link  next;
} Node;
	
typedef struct SetRep {
	int   nelems;
	Link  elems;
} SetRep;

// Function signatures

char * retrieveVal(Set, int);
Set newSet();
void disposeSet(Set);
void insertInto(Set,char *, float);
void dropFrom(Set,char *);
int  isElem(Set,char *);
int  nElems(Set);

static Link newNode(char *, float);
static void disposeNode(Link);
static int  findNode(Link,char *,Link *,Link *);

// A function to find the place of a string in the set
int nodeVal(Set set, char * val) {
	//printf("Finding node val\n");
	int i = 0;
	int imax = nElems(set);
	//printf("Elements %d\n", imax);
	char * temp = retrieveVal(set, i);
	printf("%s\n", temp);

	while (strcmp(temp, val) != 0 && i < imax - 1) {
		//printf("Iterated\n");
		i++;
		//printf("I is now %d\n", i);
		temp = retrieveVal(set, i);
		//printf("Comparing with %s\n", temp);
	}

	if (i >= imax)
		return -1;

	return i;
}

float retrieveRank(Set s, int number) {
	int i = 0;
	Link curr = s->elems;
	while (i < number) {
		curr = curr->next;
		i++;
	}
	return curr->rank;
}

char * retrieveVal(Set s, int number) {
	int i = 0;
	Link curr = s->elems;
	while (i < number) {
		curr = curr->next;
		i++;
	}
	//printf("About to return\n");
	return curr->val;
}

// newSet()
// - create an initially empty Set
Set newSet()
{
	Set new = malloc(sizeof(SetRep));
	assert(new != NULL);
	new->nelems = 0;
	new->elems = NULL;
	return new;
}

// disposeSet(Set)
// - clean up memory associated with Set
void disposeSet(Set s)
{
	if (s == NULL) return;
	Link next, curr = s->elems;
	while (curr != NULL) {
		next = curr->next;
		disposeNode(curr);	
		curr = next;
	}
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto(Set s, char *str, float rank)
{
	//printf("Inserting %s\n", str);
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (found) return; // already in Set
	//printf("Now making node\n");
	Link new = newNode(str, rank);
	s->nelems++;
	
	// Completely new section
	
	curr = s->elems;
	if (curr == NULL) {
		new->next = NULL;
		s->elems = new;
	}
	// Case for insertion at beginning of populated list
	else if (curr->rank < rank) {
		new->next = curr;
		s->elems = new;
	}

	else {
		prev = curr;
		while (curr != NULL && rank <= prev->rank && rank <= curr->rank) {
			prev = curr;
			curr = curr->next;
		}
		new->next = curr;
		prev->next = new;
	}
	
	// End new section
	
	/* (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
		//printf("added %s to start of list\n", new->val);
	}
	else {

		// NEW CODE
		curr = s->elems, prev = curr;

		while (curr != NULL && ) {
		
		}
		*/
		// END NEW CODE

		//printf("Adding elsewhere to list %s\n", new->val);
		// add into list of elems
		//new->next = prev->next;
		//prev->next = new;
}

// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	int found = findNode(s->elems,str,&curr,&prev);
	if (!found) return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	disposeNode(curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem(Set s, char *str)
{
	assert(s != NULL);
	Link curr, prev;
	return findNode(s->elems,str,&curr,&prev);
}

// nElems(Set)
// - return # elements in Set
int  nElems(Set s)
{
	assert(s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet(Set s)
{
	Link curr;
	if (s->nelems == 0)
		printf("Set is empty\n");
	else {
		printf("Set has %d elements:\n",s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode(char *str, float rank)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->val = strdup(str);
	new->next = NULL;
	new->rank = rank;
	//printf("Inserting %s with rank %f\n", str, rank);
	return new;
}

static void disposeNode(Link curr)
{
	assert(curr != NULL);
	free(curr->val);
	free(curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode(Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = curr;
	if (curr == NULL)
		printf("Initial NULL\n");
	int i = 0;
	printf("	%s\n", str);
	while (curr != NULL && !strEQ(curr->val, str)) {
		printf("Compared with %s ", curr->val);
		prev = curr;
		curr = curr->next;
		i++;

		printf("gone %d\n", i);
	}
	// If not found
	
	if (curr != NULL)
		printf("Found insertion spot at %d\n", i);
	i = 0;
	if (curr == NULL) {
		curr = list, prev = curr;
		while (curr != NULL && curr->rank <= prev->rank) {
			//printf("				Comparing %f and %f (curr prev)\n", curr->rank, prev->rank);
			prev = curr;
			curr = curr->next;
			i++;
			//printf("Now gone %d\n", i);
		}
	}
	printf("Inserted at pos %d\n", i);

	*cur = curr; *pre = prev;
	return (curr != NULL && strEQ(str,curr->val));
}
