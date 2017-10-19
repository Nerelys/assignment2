// This is the main control for this assignment in terms of testing and debugging
// Created 11/10/17
// Last edited 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pagerankFunc.h"
#include "graph.h"
#include "queue.h"
#include "invertedFunc.h"

int main (int argc, char ** argv) {
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s searchTerm...\n", argv[0]);
		return EXIT_FAILURE;
	}
	// Call pageRank, which returns a set of ordered pageranks
	// Set set = PageRank(0.85, 0.00001, 1000);
	for (int i = 0; i < nElems(set); i++) {
		char * val = retrieveVal(set, i);
		printf("%s\n", val);
	}

	// Call invertedIndex, to get a gist of where the words are
	//invertedIndex();

	return EXIT_SUCCESS;
}
