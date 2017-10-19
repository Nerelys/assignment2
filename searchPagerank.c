// This is a simple search engine
// Created 12/10/17
// Last edited: 12/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// More includes as needed
#include "pagerankFunc.h"
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "readData.h"
#include "invertedFunc.h"

int main(int argc, char ** argv) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s 'search term' ... 'search term'\n", argv[0]);
	}

	//invertedIndex();
	//Set set = PageRank(0.85, 0.00001, 1000);
	FILE * prfp = fopen("pagerankList.txt", "r");
	if (prfp == NULL) {
		fprintf(stderr, "Error: 'pagerankList.txt' does not exist\n");
		return EXIT_FAILURE;
	}

	char url[50];
	int thing;
	float readPagerank;
	Set set = newSet();

	int numLines = 0;
	char c = 'a';
	while(c != EOF) {
		c = fgetc(prfp);
		//printf("%c\n", c);
		if (c == '\n')
			numLines++;
	}
	//printf("Number of lines is %d\n", numLines);
	rewind(prfp);

	for (int i = 0; i < numLines; i++) {
		fscanf(prfp, "%s,", url);
		fscanf(prfp, "%d,", &thing);
		fscanf(prfp, "%f", &readPagerank);
		url[strlen(url) - 1] = '\0';
		insertInto(set, url, readPagerank);
		//printf("---> %s %.7f\n", url, readPagerank);
	}
	fclose(prfp);

	//Graph g = newGraph(20);
	//Set s = newSet();
	//printf("In search: %d\n", nElems(set));
	//BSTree tree = readData(s, g, set);

	Set tallySet = newSet();

	// Try to accumulate terms
	// This cycles through all of our search terms
	// and gradually adds them to a set that tallies our results
	for (int i = 1; i < argc; i++) {
		char * temp = argv[i];
		//printf("Accumulating for %s\n", temp);
		accumulateTerms(temp, tallySet);
	}

	/*
	for (int i = 0; i < nElems(set); i++) {
		printf("%s\n", retrieveVal(set, i));
	}
	*/

	int loopCount = 0;
	while (loopCount < nElems(set)) {
		//printf("Looping\n");
		char * url = retrieveVal(set, loopCount);
		//printf("Url is %s with %d at pos %d\n", url, nElems(set), loopCount);
		if (!isElem(tallySet, url)) {
			//printf("Dropped %s\n\n", url);
			dropFrom(set, url);
			loopCount = 0;
		}
		else {
			//printf("Not\n");
			loopCount++;
		}
	}
	
	//printf("LEAVING\n");

	int i = nElems(tallySet);
	// Due to how I modified the insertInto function, the tallyset is
	// hopefully already ordered in terms of term frequency

	int tracker[i];
	// This particular function orders by pagerank IF the tally number is the same
	// otherwise, otherwise on basis of tally count
	// This is partially to catch any unordered set slipping through
	orderSet(set, tallySet, tracker);
	/*
	for (int k = 0; k < i; k++) {
		char * temp = retrieveVal(tallySet, k);
		float count = retrieveRank(tallySet, k);
		insertInto(finalSet, temp, count);
	}
	*/

	//printf("\nFinal results:\n");
	if (i > 30)
		i = 30;

	for (int k = 0; k < i; k++) {
		//printf("Tracker is %d\n", tracker[k]);
		char * temp = retrieveVal(tallySet, tracker[k]);
		//float count = retrieveRank(set, nodeVal(set, temp));
		//float rank = retrieveRank(tallySet, tracker[k]);
		printf("%s\n", temp);
	}

	//disposeSet(finalSet);
	disposeSet(tallySet);
	disposeSet(set);
	//disposeSet(s);

	// TODO: dispose all sets and graphs at end
	//printf("\nAll done!\n");
	return EXIT_SUCCESS;
}
