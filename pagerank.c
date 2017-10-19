// Literally just a crappy main to get pageranks...
// Created 10/10/17
// Last edited:	  10/10/17
// Creator: Angus Blake

#include <stdlib.h>
#include <stdio.h>
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "BSTree.h"
#include "pagerankFunc.h"

int main (int argc, char ** argv) {
	printf("Generating pageranks:\n");

	if (argc < 4) {
		printf("Usage: %s dampingFactor pagerankDifference maxIterations\n", argv[0]);
		return EXIT_FAILURE;
	}

	PageRank(atof(argv[1]), atof(argv[2]), atoi(argv[3]));

	printf("Successfully wrote to pagerankList.txt\n");
	return EXIT_SUCCESS;
}