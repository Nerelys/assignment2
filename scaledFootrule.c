// A program designed to compare multiple ranking files at once!
// Written 20/10/17
// Last edited 20/10/17

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"

int main(int argc, char ** argv) {
	// First thing's first, make sure that we read the files in

	if (argc < 2) {
		fprintf(stderr, "Usage: %s rankfile ... rankfile\n", argv[0]);
		return EXIT_FAILURE;

	}

	// Make a thing to manage our sets
	Set setList[argc - 1];
	Set alphaSet = newSet();

	int i;
	FILE * fp;
	// A loop for reading in the .txt files
	for (i = 1; i < argc; i++) {
		//printf("%s\n", argv[i]);
		fp = fopen(argv[i], "r");
		if (fp == NULL) {
			//fprintf(stderr, "Error: File '%s' not found. Are you in the right directory?\n", argv[i]);
			fclose(fp);
			return EXIT_FAILURE;
		}

		int numLines = 0;
		char c = 'a';
		while(c != EOF) {
		c = fgetc(fp);
		//printf("%c\n", c);
		if (c == '\n')
			numLines++;
		}
		//printf("Number of lines is %d\n", numLines);
		rewind(fp);

		Set set = newSet();
		setList[i - 1] = set;

		float j = 0;
		while (j < numLines) {
			char url[50];
			fscanf(fp, "%s", url);
			//printf("Found %s\n", url);
			insertInto(setList[i - 1], url, j + 1);
			insertInto(alphaSet, url, -1);
			j++;
		}
		// Close the file afterwards, be nice!
		fclose(fp);
	}
	//showSet(setList[0]);
	//showSet(setList[1]);
	//showSet(alphaSet);

	// Calculate the initial unchanged p-values
	//int listEnable[argc - 1]; // an array to check whether a set is eligibe for p calc.
	float pVals[nElems(alphaSet)];
	for (int j = 0; j < nElems(alphaSet); j++) {
		pVals[j] = 0;
	}
	//printf("To test pVal init %f and fringe case %f\n", pVals[0], pVals[4]);

	int pSeries[nElems(alphaSet)];
	for (int j = 0; j < nElems(alphaSet); j++) {
		pSeries[j] = j + 1;
		//printf("The pSeries thingo is %d\n", pSeries[j]);
	}

	for (i = 0; i < nElems(alphaSet); i++) {
		int j;
		//printf("%d %d\n", listEnable[0], listEnable[1]);
		for (j = 0; j < argc - 1; j++) {
			if (isElem(setList[j], retrieveVal(alphaSet, i))) {
				//listEnable[j] = 1;
				char * gba = retrieveVal(alphaSet, i);
				//printf("PSERIES TEST I THEN 1 %d %d\n", pSeries[i], pSeries[1]);

				//printf("It's %s\n", gba);
				//printf("The rank is %f, nElems is %d, pSeries is %d, nElems alpha is %d\n", retrieveRank(setList[j], nodeVal(setList[j], gba)), 
					//nElems(setList[j]), pSeries[i], nElems(alphaSet));
				//printf("First frac is %f second is %f\n", retrieveRank(setList[j], nodeVal(setList[j], gba)) / nElems(setList[j]), (1.0 * pSeries[i]) / nElems(alphaSet));
				float tempShit = retrieveRank(setList[j], nodeVal(setList[j], gba))/nElems(setList[j]) - ((1.0*pSeries[i])/nElems(alphaSet));
				//printf("Immediate got ---> %f\n", tempShit);
				if (tempShit < 0) {
					tempShit = tempShit * -1;
				}
				pVals[i] = pVals[i] + tempShit;
				//printf("-> %d -> pVals[%d] is now %f\n", j, i, pVals[i]); 
 				//printf("-> %d\n", listEnable[j]);
			}
			//printf("Got here\n");
			/*
			else {
				listEnable[j] = 0;
				//printf("-> %d\n", listEnable[j]);
			}
			*/
		}
		//pVals[i] = pVals[i];
	}

	//printf("\n");
	
	// At this point the contents of the files have all been put into differing arrays
	// and we have the INITIAL weighted values
	// Note: This is really only initial. I plan on doing a plain ol' 1-2 switch sort
	// That only compares switching two p values 
	// Yes it may not be the BEST, but this is just a first try!
	i = 0;
	while (i < nElems(alphaSet)) {
		//printf("\nLooping %d\n", i);
		//for (int temp = 0; temp < nElems(alphaSet); temp++)
			//printf(">>> pSeries[%d] -> %d\n", temp, pSeries[temp]);
		//for (int temp = 0; temp < nElems(alphaSet); temp++)
			//printf("--- pVals[%d] -> %f\n", temp, pVals[temp]);
		float sum = 0;
		for (int temp = 0; temp < nElems(alphaSet); temp++) {
			sum = sum + pVals[temp];
		}
		//printf("Current best distance is %f\n", sum);
		int j = 0;
		while (j < nElems(alphaSet)) {
			//printf("Hi\n");
			if (j <= i) {
				j++;
				continue; // Obviously don't compare a set with itself!
						// Also if we've done the same comparison in reverse skip
			} 
			int k;
			float tempShit = 0; 
			float tempShit2 = 0;
			for (k = 0; k < argc - 1; k++) { 
				char * gba = retrieveVal(alphaSet, i);
				//printf("It's %s\n", gba);
				//printf("The rank is %f, nElems is %d, pSeries is %d, nElems alpha is %d\n", retrieveRank(setList[k], nodeVal(setList[k], gba)), nElems(setList[k]), pSeries[j], nElems(alphaSet));
				//printf("First frac is %f second is %f\n", retrieveRank(setList[k], nodeVal(setList[k], gba)) / nElems(setList[k]), (1.0 * pSeries[j]) / nElems(alphaSet));
				float realTemp = retrieveRank(setList[k], nodeVal(setList[k], gba))/nElems(setList[k]) - ((1.0*pSeries[i])/nElems(alphaSet));
				if (realTemp < 0) {
					realTemp = realTemp * -1;
				}
				tempShit = tempShit + realTemp;
				//printf("Immediate got1 ---> %f\n", tempShit);
			}

			for (k = 0; k < argc - 1; k++) {
				char * gba = retrieveVal(alphaSet, j);
				//printf("It's %s\n", gba);
				//printf("Checking second pSeries %d\n", pSeries[i]);
				//printf("The rank is %f, nElems is %d, pSeries is %d, nElems alpha is %d\n", retrieveRank(setList[k], nodeVal(setList[k], gba)), 
					//nElems(setList[k]), pSeries[i], nElems(alphaSet));
				float realTemp = retrieveRank(setList[k], nodeVal(setList[k], gba))/nElems(setList[k]) - ((1.0*pSeries[i])/nElems(alphaSet));
				if (realTemp < 0) {
					realTemp = realTemp * -1;
				}
				tempShit2 = tempShit2 + realTemp;
				//printf("Immediate got2 ---> %f\n", tempShit2);
			}

			//printf("		CHECKSUM %f\n", (tempShit2 + tempShit));
			if ((tempShit + tempShit2) < (pVals[i] + pVals[j])) {
				//printf("****************** Less than %f\n", pVals[i] + pVals[j]);
				//printf("Lower sum is %f compares to %f\n", tempShit + tempShit2, pVals[i] + pVals[k]);
				//printf("Swapping %d and %d\n", i, j);
				//float vTemp = pVals[j];
				pVals[j] = tempShit2;
				pVals[i] = tempShit;
				int pTemp = pSeries[j];
				pSeries[j] = pSeries[i];
				pSeries[i] = pTemp;
				i = 0;
				j = 0;
			}
			j++;
		}
		i++;
	}

	float sum = 0;
	for (int temp = 0; temp < nElems(alphaSet); temp++) {
			sum = sum + pVals[temp];
		}
	printf("%f\n", sum);
	//printf("\n\n\n");

	for (i = 0; i < nElems(alphaSet); i++) {
		printf("%s\n", retrieveVal(alphaSet, pSeries[i] - 1));
	}

	return EXIT_SUCCESS;
}