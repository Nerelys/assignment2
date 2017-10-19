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
#include "tfidf.h"	

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

	FILE * collectionfp = fopen("collection.txt", "r");
	if (collectionfp == NULL) {
		fprintf(stderr, "Error: collection.txt not found! Are you in the right directory?\n");
		return EXIT_FAILURE;
	}

	int vertices = 0;
	char collWord[50];
	fscanf(collectionfp, "%s", collWord);
	while (!feof(collectionfp)) {
		vertices++;
		fscanf(collectionfp, "%s", collWord);
	}
	//printf("%d URLs found\n", vertices);
	// Do invertedIndex first as we need invertedIndex.txt to
	// reference and get the tf-idf

	// Then get a tallySet as we do need to base our initial
	// ranking off no. of occurences of a word

	Set tallySet = newSet();

	char argTemp[50];
	for (int i = 1; i < argc; i++) {
		strcpy(argTemp, argv[i]);
		wordTrim(argTemp);
		//printf("%s\n", argTemp);
		//printf("Accumulating for %s\n", temp);
		accumulateTerms(argTemp, tallySet);
	}

	Set tfidfSet = newSet();
	int elems = nElems(tallySet);
	//int vertices = nVertices(g);

	for (int i = 0; i < elems; i++) {
		double loopCount = 0;
		char * page = retrieveVal(tallySet, i);
		//printf("For page %s: ", page);
		for (int j = 1; j < nElems(tallySet) + 1; j++) {
			//printf("**\nFor %s\n", argv[j]);
			//printf("It's %s\n", retrieveVal(tallySet, j));
			loopCount = loopCount + (getIdf(wordTrim(argv[j]), vertices) * getTf(page, wordTrim(argv[j])));
			//printf("%f\n", loopCount);
		}
		//printf("\nFinal tfidf is %lf\n", loopCount);
		insertInto(tfidfSet, page, loopCount);
	}
	
	// Now just as we did previously order this set with primary
	// factor being no. of terms, and secondary being tfidf
	int tracker[elems];

	orderSet(tfidfSet, tallySet, tracker);

	//printf("\nFinal tfidf rank results:\n");

	for (int k = 0; k < elems; k++) {
		char * temp = retrieveVal(tallySet, tracker[k]);
		double count = retrieveRank(tfidfSet, nodeVal(tfidfSet, temp));
		float rank = retrieveRank(tallySet, tracker[k]);
		printf("%s %.0f %.6f\n", temp, rank, count);
	}

	//printf("All done!\n");
	return EXIT_SUCCESS;
}
