// This is a simple search engine
// Created 12/10/17
// Last edited: 12/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// More includes as needed
#include "pagerank.h"
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "readData.h"
#include "inverted.h"

int main(int argc, char ** argv) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s 'search term' ... 'search term'\n", argv[0]);
	}

	invertedIndex();
	Set set = PageRank(0.85, 0.00001, 1000);
	Graph g = newGraph(20);
	Set s = newSet();
	//printf("In search: %d\n", nElems(set));
	BSTree tree = readData(s, g, set);

	//printf("Output tree\n");
	// A loop to:
	// 	-> Get tree NOT ANY MORE
	// 	-> Get pageranks NOT ANY MORE
	// 	-> Cross reference
	for (int i = 1; i < argc; i++) {
		char * temp = argv[i];
		//printf("\n*****\n%s\n", temp);
		outputTree(tree, temp);
	}

	Set tallySet = newSet();

	// Try to accumulate terms
	for (int i = 1; i < argc; i++) {
		char * temp = argv[i];
		//printf("Accumulating for %s\n", temp);
		accumulateTerms(tree, temp, tallySet);
	}
	
	int i = nElems(tallySet);
/*	for (int k = 0; k < i; k++){
		char * temp = retrieveVal(tallySet, k);
		float count = retrieveRank(tallySet, k);
		//printf("--> %s with count %f\n", temp, count);
	}
*/
	int tracker[i];
	orderSet(set, tallySet, tracker);
	/*
	for (int k = 0; k < i; k++) {
		char * temp = retrieveVal(tallySet, k);
		float count = retrieveRank(tallySet, k);
		insertInto(finalSet, temp, count);
	}
	*/
	printf("\nFinal results:\n");
	
	for (int k = 0; k < i; k++) {
		char * temp = retrieveVal(tallySet, tracker[k]);
		float count = retrieveRank(tallySet, tracker[k]);
		printf("--> %s with count %f\n", temp, count);
	}

	//disposeSet(finalSet);
	disposeSet(tallySet);
	disposeSet(s);

	// TODO: dispose all sets and graphs at end
	printf("\nAll done!\n");
	return EXIT_SUCCESS;
}
