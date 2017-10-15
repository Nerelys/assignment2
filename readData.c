// For reading data
// Created 11/10/17
// Last edited 11/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "readData.h"
#include "BSTree.h"

BSTree readData (Set set, Graph g, Set pagerankSet) {
	// Telling us if we already have a working pagerank set
	//printf("THIS IS IMPORTANT ----------------------------> %d\n", nElems(pagerankSet));
	int setFlag = 0;
	if (nElems(pagerankSet) != 0)
		setFlag = 1;

	/*
	if (setFlag == 1) {
		int elems = nElems(pagerankSet);
		for (int i = 0; i < elems; i++) {
			char * temp = retrieveVal(pagerankSet, i);
			float rank = retrieveRank(pagerankSet, i);
			//printf("Testing to get %s and %f\n", temp, rank);
		}
	}
	*/

   FILE * fp;
	//Graph g = newGraph(20);
	Queue q = newQueue();
	BSTree tree = newBSTree();
	//char set[20][20];
	char word[100];			// For repeatedly reading words
	char word2[100];			// For reading only 'section-1' or 'section-2'

	fp = fopen("collection.txt", "r");
	while (!feof(fp)) {				// The usual loop
		fscanf(fp, "%s", word);		// Pop off queue as long as we have tasks
		enterQueue(q, word);
		//showQueue(q);
	}
	//printf("Temporary ending --------------------------\n");
	//return EXIT_SUCCESS;

	while (!emptyQueue(q)) {
		char * vertex = leaveQueue(q);	// Vertex is url without '.txt'
		char url[20];							// Another array for the '.txt'
		strcpy(url, vertex);
		strcat(url, ".txt");
		//printf("Chosen url is %s\n", url);	// Now url has the '.txt'
		insertInto(set, vertex, 0);			// Insert this into the 'seen' set
		fp = fopen(url, "r");				// early or it becomes a problem ;_;

		while(!feof(fp)) {
			//printf("Scanning for section\n");
			fscanf(fp, "%s", word);
			fscanf(fp, "%s", word2);

			//printf("Word 1: %s, and word 2: %s\n", word, word2);	// Figure out which section
			if (strcmp(word, "Section-2") == 0) {
				break;
			}

			if (strcmp(word, "#start") == 0 && strcmp(word2, "Section-1") == 0) {
				fscanf(fp, "%s", word);
				while (strcmp(word, "#end") != 0) {		// As long as we're in section 1
					//printf("%s\n", word);
					if (strcmp(word, vertex) != 0) {
						addEdge(g, vertex, word);
						insertInto(set, word, 0);
						//showBSTree(tree);
						//showGraph(g, 1);
						//printf("\n");
					}
					fscanf(fp, "%s", word);
				}
				fscanf(fp, "%s", word);
				// Check for correct exit syntax
				if (strcmp(word, "Section-1") != 0) {
					fprintf(stderr, "Invalid exit situation: Require '#end Section-1'\n");
					//return EXIT_FAILURE;
				}
				//printf("Exit section 1\n");
			}

			// Section-2 start
			else if (strcmp(word, "#start") == 0 && strcmp(word2, "Section-2") == 0) {
				fscanf(fp, "%s", word);
				//printf("\n");
				while (strcmp(word, "#end") != 0) {
					//printf("%s ", word);
					wordTrim(word);
					//printf("Inserting %s with %s --------\n", word, vertex);
					if (setFlag == 1) {
						//printf("___________________Inserting as flag 1\n");
						tree = BSTreeInsert(tree, word, vertex, pagerankSet);
					}
					else
						tree = BSTreeInsert(tree, word, vertex, set);
					//tree = BSTreeInsert(tree, word, vertex, pagerankSet);
					//showBSTree(tree);
					fscanf(fp, "%s", word);
				}
				//printf("\n\n");
				fscanf(fp, "%s", word);	// Scan for 'Section-2'
				// Check for correct syntax
				if (strcmp(word, "Section-2") != 0) {
					fprintf(stderr, "Invalid exit situation: Require '#end Section-2'\n");
					//return EXIT_FAILURE;
				}
				//printf("Exit section 2 with word %s\n", word);
			}

			else {
				printf("Incorrect word: %s\n", word);
				fprintf(stderr, "Invalid hook: Looking for '#start Section-x'\n");
				//return EXIT_FAILURE;
			}
		}
		fclose(fp);
	}
	//showBSTree(tree);
	return tree;
}

char * wordTrim(char * word) {
	int len = strlen(word);

	for (int i = 0; i < len; i++) {
		//printf("Trimming %c ", word[i]);

		// Case 1: a lower case character
		if (word[i]	>= 'a' && word[i] <= 'z') {
		}

		// Case 2: upper case
		else if (word[i] >= 'A' && word[i] <= 'Z')
			word[i] = word[i] + ('a' - 'A');		// Offset necessary to change upper to lower case

		// Case 3: Random punctuation
		else if (word[i] != ' ') {
			word[i] = '\0';			// Chuck in null terminator because this
		}									// puctuation is always at end of word
		
		// This case if for spaces at beginning or end
		else if (word[i] == ' ') {
			if (i != 0)
				word[i] = '\0';		// Space at end of word
			else {
				for (int k = 0; k < len - 1; k++) {
					//printf("Replacing %c with %c\n", word[k], word[k + 1]);
					word[k] = word[k + 1];
				}
			}

		}
		else
			printf("Unknown character case\n");

		//printf("to %c\n", word[i]);
	}

	return word;
}
