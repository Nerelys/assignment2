// A file with supporting code for searchTfIdf
//
// Created 17/10/17
// Last edited 17/10/17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tfidf.h"

double getIdf(char * word, int vertices) {

FILE * fp = fopen("invertedIndex.txt", "r");

	// Get our total number of URL's from the file header
	char temp[100];
	//fscanf(fp, "%s", temp);
	int urlNum = vertices;
	// Quick bit of code to read the header
	/*if (strcmp(temp, "#URLNUM") == 0) {
		//int urlNum[1];
		fscanf(fp, "%lf", &urlNum);
		fscanf(fp, "%s", temp);
		if (strcmp(temp, "#ENDURLNUM") != 0) {
			fprintf(stderr, "Invalid exit header!\n");
			return EXIT_FAILURE;
		}
	}
	else {
		fprintf(stderr, "Invalid header!\n");
		return EXIT_FAILURE;
	}
	*/
	// Now we have the total number of files, find the IDF
	fscanf(fp, "%s", temp);
	while (strcmp(temp, word) != 0) {
		fscanf(fp, "%s", temp);
		//printf("%s\n", temp);
	}

	// Now read the rest of the line (which contains the URL's)
	//char newlineChar[1];
	//newlineChar[0] = ' ';
	char buffer[1000];
	double urlCount = 0;
	int length = sizeof(buffer);

	// Move the file pointer forward by one to
	// counter the sneaky space
	// Trust me, it's necessary for what's going on here
	fseek(fp, 1, SEEK_CUR);

	// Use fgets to get the whole line because it's easy and nice
	fgets(buffer, length, fp);
	char buff = buffer[0];
	int pos = 0;
	//printf("Start\n");

	// Within the inner loop, start making a string for the url
	// so that we can parse it in and get tf at the same time!

	//char dest[50];		// 50 seems bit enough for a url, right?
	//int destpos = 0;

	// So I read the spec, and I don't actually need to parse in
	// per URL, so that's a thing...
	// I removed dest and destpos accordigly
	while (buff != '\n') {
		//printf("Outer loop\n");
		while (buff != ' ') {
			//printf("Inner loop %c\n", buff);
			pos++;
			buff = buffer[pos];
			//destpos++;
			//dest[destpos] = buff;
		}
		//dest[destpos] = '\0';
		//printf("To test if string conversion worked %s\n", dest);
		
		// Parse dest into getTf because we have it and it works
		// Get off my back, man!
		// (Removed because I'm an idiot and didn't read spec correctly)
		//getTf(dest, word);

		//destpos = 0;
		urlCount++;
		pos++;
		buff = buffer[pos];
	}

	double idf = (urlNum/urlCount);
	//printf("input for log is %lf\n", idf);
	idf = log10(idf);
	//printf("now %lf\n", idf);
	//printf("urlNum is %d and urlcount is %lf\n", urlNum, urlCount);
	//printf("Term IDF for %s is %lf\n", word, idf);
	// Close our file, no littering
	fclose(fp);

	return idf;
}

// Get the tf for the current page
// May change the return value later to return the Tf value
// Or not, whatever works really
double getTf(char * file, char * word) {
	// So this is gonna be a bit like readData.c
	// This time, I just want to open the file and read through
	// For all the words, and only tally the word I want

	// Gotta add the '.txt' to our URL
	//printf("Parsed into getTf is %s\n", file);
	char url[50];
	strcpy(url, file);
	strcat(url, ".txt");
	//printf("Now it is %s\n", url);

	FILE * filePointer = fopen(url, "r");

	double count = 0;
	double wordCount = 0;
	char tempWord[100];

	fscanf(filePointer, "%s", tempWord);

	// First find beginning of 'word' section
	while (strcmp(tempWord, "Section-2") != 0) 
		fscanf(filePointer, "%s", tempWord);

	// Now scan the words
	// MAKE SURE TO TRIM THEM
	// ie. normalise them
	// Or else they won't register and it'll screw up the data

	while(strcmp(tempWord, "#end") != 0) {
		char * normalised = wordTrim(tempWord);
		if(strcmp(normalised, word) == 0) 
			count++;
		wordCount++;
		fscanf(filePointer, "%s", tempWord);
	}

	wordCount = wordCount - 1;
	double tf = (count/wordCount);
	//printf("Total number of words in %s is %lf\n", file, wordCount);
	//printf("Terms found = %lf, tf = %lf\n", count, tf);

	// As always, clean up after ourselves!
	fclose(filePointer);
	return tf;
}