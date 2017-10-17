// This is the main control for this assignment in terms of testing and debugging
// Created 11/10/17
// Last edited 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pagerank.h"
#include "graph.h"
#include "queue.h"
#include "inverted.h"

int main (int argc, char ** argv) {
	
	// Call pageRank, which returns a set of ordered pageranks
	Set set = PageRank(0.85, 0.00001, 1000);
	printf("\n");
	for (int i = 0; i < nElems(set); i++) {
		char * val = retrieveVal(set, i);
		float rank = retrieveRank(set, i);
		printf("Outside pagerank: %s and %.7f\n", val, rank);
	}

	// Call invertedIndex, to get a gist of where the words are
	//invertedIndex();

	return EXIT_SUCCESS;
}
