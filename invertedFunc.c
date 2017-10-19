// A file to find URL's for a given input word
// Created 11/10/17
// Last edited: 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "invertedFunc.h"
#include "graph.h"
#include "set.h"
#include "BSTree.h"
#include "readData.h"

BSTree invertedIndex() {
	//printf("Everything is working so far!\n");
	Graph g = newGraph(20);
	Set set = newSet();
	BSTree tree = newBSTree();
	
	Set temp = set;
	tree = readData(set, g, temp);

	//showGraph(g, 1);

	//printf("About to show\n");
	FILE * fp = fopen("invertedIndex.txt", "w");		// Erase existing
	//	fprintf(fp, "#URLNUM %d #ENDURLNUM\n", nVertices(g));
	fclose(fp);

	showBSTree(tree);
	return tree;
	//return;
}
