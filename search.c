// This is the main control for this assignment in terms of testing and debugging
// Created 11/10/17
// Last edited 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pagerank.h"
#include "graph.h"
#include "queue.h"

int main (int argc, char ** argv) {
	
	// Call pageRank, which I've made a void return
	PageRank(0.85, 0.00001, 1000);

	return EXIT_SUCCESS;
}
