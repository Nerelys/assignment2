// A program designed to fetch both the term frequency
// and the inverse document frequency for a given search query
// Written by Angus and Geyan
// Created 17/10/17
// Last edited: 17/10/17

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "searchTfIdf.h"
#include "tfidf.h"	// TODO create this file

int main(int argc, char ** argv) {
	// In order to get the term frequency, we need to tally the amount of
	// times a word is seen in a document
	//
	// Then to find IDF we need to search ALL documents and find 

	// Do a quick check for number of inputs
	if (argc < 2) {
		fprintf(stderr, "Usage: %s searchTerm searchTerm2 ...\n", argv[0]);
		return EXIT_FAILURE;
	}

	invertedIndex();
	// These function are for accumulateTerms()
	Set s = newSet();
	Set trashSet = newSet();
	Graph g = newGraph(20);
	BSTree tree = readData(s, g, trashSet);

	// Do invertedIndex first as we need invertedIndex.txt to
	// reference and get the tf-idf

	// Then get a tallySet as we do need to base our initial
	// ranking off no. of occurences of a word

	Set tallySet = newSet();

	for (int i = 1; i < argc; i++) {
		char * temp = argv[i];
		accumulateTerms(tree, temp, tallySet);
	}

	Set tfidfSet = newSet();
	int elems = nElems(tallySet);

	for (int i = 0; i < elems; i++) {
		double loopCount = 0;
		char * page = retrieveVal(tallySet, i);
		printf("For page %s: ", page);
		for (int j = 1; j < argc; j++) {
			loopCount = loopCount + (getIdf(argv[j]) * getTf(page, argv[j]));
		}
		printf("\nFinal tfidf is %lf\n", loopCount);
		insertInto(tfidfSet, page, loopCount);
	}
	
	// Now just as we did previously order this set with primary
	// factor being no. of terms, and secondary being tfidf
	int tracker[elems];

	orderSet(tfidfSet, tallySet, tracker);

	printf("\nFinal tfidf rank results:\n");

	for (int k = 0; k < elems; k++) {
		char * temp = retrieveVal(tallySet, tracker[k]);
		double count = retrieveRank(tfidfSet, k);
		float rank = retrieveRank(tallySet, tracker[k]);
		printf("%s with rank %.7f and tfidf %.6f\n", temp, rank, count);
	}

	printf("All done!\n");
	return EXIT_SUCCESS;
}
