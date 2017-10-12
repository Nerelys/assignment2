// A file to find URL's for a given input word
// Created 11/10/17
// Last edited: 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inverted.h"
#include "graph.h"
#include "set.h"
#include "BSTree.h"
#include "readData.h"

void invertedIndex(void) {
	printf("Everything is working so far!\n");
	Graph g = newGraph(20);
	Set set = newSet();
	BSTree tree = newBSTree();

	tree = readData(set, g);

	showGraph(g, 1);

	printf("About to show\n");
	showBSTree(tree);
	return;
}
