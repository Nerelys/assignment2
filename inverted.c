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

int main(int argc, char ** argv) {
	printf("Compiling inverted Index...\n\n");
	invertedIndex();

	printf("Jobs done!\n");
	return EXIT_SUCCESS;
}