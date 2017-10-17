// A file designed to extract URL's and word data
// and then rank them according to outbound URL usage
// Created 10/10/17
// Last edited:	  10/10/17
// Creator: Angus Blake

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
#include "set.h"
#include "queue.h"
#include "pagerank.h"
#include "readData.h"
#include "BSTree.h"

// First and foremost I want to test if I can extract the data!
Set PageRank(float d, float diffPR, int maxIterations) {
	printf("Running pagerank\n");

	// All of this is to read the files and get a graph rep.
	Set set = newSet();
	Set throwawaySet = newSet();
	//BSTree tree = newBSTree();
	Graph g = newGraph(20);
	readData(set, g, throwawaySet);
	// At this point, the graph is fully built and all URL's are checked
	// The task changes to getting the number of links to and from URL's,
	// and then building the pagerank

	printf("\n\nFetching outbound links...\n");
	int elements = nElems(set);
	float outbound[20] = {0};
	// To cut is short, this whole block simply fetches the number of outbound links
	// for each URL, and stores the corresponding val. in outbound[]
	for (int i = 0; i < elements; i++) {
		char * src = retrieveVal(set, i);
		for (int k = 0; k < elements; k++) {
			if (i == k) continue;
			//printf("i %d k %d\n", i, k);
			char * dest = retrieveVal(set, k);
			//printf("Checking word: %s\n", dest);
			if (isConnected(g, src, dest))
				outbound[i]++;
		}
		if (outbound[i] == 0)
			outbound[i] = 0.5;
	}
	for (int j = 0; j < elements; j++)
		printf("Outbound for vert. %d is %f\n", j, outbound[j]);

	// The counterpart to the outbound loop, this block fetches all INbound links for each URL
	// and stores them in inbound[]
	printf("\n\nFetching inbound links...\n");
	float inbound[20] = {0};
	for (int i = 0; i < elements; i++) {
		char * dest = retrieveVal(set, i);
		for (int k = 0; k < elements; k++) {
			if (i == k) continue;
				char * src = retrieveVal(set, k);
			if (isConnected(g, src, dest))
				inbound[i]++;
		}
	}
	for (int j = 0; j < elements; j++)
		printf("Inbound for vert. %d is %f\n", j, inbound[j]);

	// Quick test on WValue function
	//printf("\n");
	//printf("Testing W(in) of 0 -> 1: %f\n", WValue(g, inbound, 0, 1, set));
	//printf("Should see 0.25\n");
	//printf("\n");
	//printf("Testing W(out) of 0 -> 1: %f\n", WValue(g, outbound, 0, 1, set));
	//printf("Should see approx. 0.1111\n");

	// Now a loop to calc. pagerank
	// Make 2 arrays, one being an pageRank(t+1)
	float pageRank[elements];
	float newPageRank[elements];
	for (int i = 0; i < elements; i++) {
		pageRank[i] = 1.0/elements;
		//printf("		Pagerank[%d] = %f\n", i, pageRank[i]);
	}

	//int maxIterations = 1000;
	int iteration = 0;
	float diff = diffPR;
	//float d = 0.85;

	while (iteration < maxIterations && diff >= diffPR) {
		iteration++;

		// Loop to update pageRank(t+1)
		for (int i = 0; i < elements; i++) {

			// Secondary loop to get M(pi)
			// (All nodes with outgoing links to pi)
			float weightedWValue = 0;
			//printf("\n				*********************\n");
			for (int j = 0; j < elements; j++) {
				if (i == j) continue;
				char * src = retrieveVal(set, j);
				char * dest = retrieveVal(set, i);
				if (!isConnected(g, src, dest)) continue;
				// If they ARE connected...
				weightedWValue = weightedWValue + (pageRank[j]) * (WValue(g, inbound, j, i, set) *
					WValue(g, outbound, j, i, set));
				//printf("For %d to %d, new weightedVal is %f\n", j, i, weightedWValue);
			}
			newPageRank[i] = ((1.0 - d)/elements) + (d * weightedWValue);
			//printf("new pagerank for %d is %.7f\n", i, newPageRank[i]);
		}
		// After we've completely updated newPageRank, run our changes on diff
		diff = 0;
		for (int i = 0; i < elements; i++) {
			float temp = newPageRank[i] - pageRank[i];
			if (temp < 0) // Quick correction for Abs()
				temp = (-1 * temp);
			//printf("temp is %f\n", temp);	// Just diagnostic info
			diff = diff + temp;
			//printf("%f and new %f\n", pageRank[i], newPageRank[i]);
			pageRank[i] = newPageRank[i];
			//printf("					%f\n", diff);
			//printf("%f and %f\n", pageRank[i], newPageRank[i]);
		}
		//printf("\n					*****\n");
	}

	//printf("\n");

	Set rankSet = newSet();
	// Print out unsorted final pageranks
	for (int i = 0; i < elements; i++) {
		char * result = retrieveVal(set, i);
		//printf("Final results for %s is %.7f\n", result, newPageRank[i]);
		insertInto(rankSet, result, newPageRank[i]);
	}

/*
	// Print out SORTED final pageranks
	int first = 1;
	// A quick and easy bubblesort on the final product
	// This array is only to keep track of what was swapped for naming purposes
	// and troubleshooting
	int tracker[elements];
	for (int i = 0; i < elements; i++) {
		tracker[i] = i;
	}

	while (first < elements) {
		if (newPageRank[first] > newPageRank[first-1]) {
			float temp = newPageRank[first];
			newPageRank[first] = newPageRank[first-1];
			newPageRank[first-1] = temp;

			int temp2 = tracker[first];
			tracker[first] = tracker[first-1];
			tracker[first-1] = temp2;

			first = 1;
		}
		else {
			first++;
		}
	}
	printf("\n");
	for (int i = 0; i < elements; i++) {
		char * url = retrieveVal(set, tracker[i]);
		printf("Rank %d ---> %s, with rankValue %.7f\n", i+1, url, newPageRank[i]);
	}
	
	// Now create a final set with the pageranks in order!
	
	for (int i = 0; i < elements; i++) {
		char * val = retrieveVal(set, tracker[i]);
		//float rank = retrieveRank(set, i);
		insertInto(rankSet, val, newPageRank[i]);
	}
	*/
/*
	for (int i = 0; i < elements; i++) {
		char * val = retrieveVal(rankSet, i);
		float rank = retrieveRank(rankSet, i);
		//printf("Got %s and %.7f\n", val, rank);
	}
*/
	//disposeSet(set);
	
	//rankSet = orderSet(rankSet);
	/*
	for (int i = 0; i < nElems(rankSet); i++) {
		char * temp = retrieveVal(rankSet, i);
		float rank = retrieveRank(rankSet, i);
		printf("Final sorted result -> %s rank %f\n", temp, rank);
	}
	*/
	return rankSet;
}

// A function to compilate the W() value of a link
// Elements is the size of the inbound/outbound array
// Parse inbound[] or outbound[] for each task
float WValue(Graph g, float * links, int src, int dest, Set set) {
	float W = 0;
	float ip1 = 0; float ip2 = 0;
	char * srcVal;
	char * destVal;

	// We need the amount of outbound links of the target URL
	// as well as all outbound links of all URL's connected to the source
	// So we set ip1 to outbound[target], ezpz
	// But then we need to check for connections to the source...
	ip1 = links[dest];
	//printf("ip1: %f\n", ip1);

	// A loop to check for connections through the source URL
	for (int i = 0; i < nElems(set); i++) {
		srcVal = retrieveVal(set, src);
		destVal = retrieveVal(set, i);
		if (!isConnected(g, srcVal, destVal)) continue;		// If not connected, try again
		ip2 = ip2 + links[i];
		//printf("ip2 update: %f\n", ip2);
	}

	srcVal = retrieveVal(set, src);
	destVal = retrieveVal(set, dest);
	W = (ip1/(ip2));
	//printf("%f\n", (ip2));
	//printf("W val of %s to %s is %f\n\n", srcVal, destVal, W);

	return W;

}
