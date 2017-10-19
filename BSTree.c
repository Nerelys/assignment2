// BSTree.c ... implementation of binary search tree ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "BSTree.h"
#include "queue.h"
#include "set.h"

typedef struct BSTNode *BSTLink;

typedef struct BSTNode {
char * value;
	BSTLink left, right;
	Set set;
} BSTNode;

void accumulateTerms(char * term, Set tallySet) {
	// Find all search terms and add the respective URL's to a list?
	// And do something to count the amount of times they appear
	// This code will look very similar to outputTree()
	/*
	if (t == NULL) {
		return;
	}
	if (strcmp(term, t->value) < 0) {
		return accumulateTerms(t->left, term, tallySet);
	}
	if (strcmp(term, t->value) > 0) {
		return accumulateTerms(t->right, term, tallySet);
	}
	*/ // Changed to work with invertedIndex.txt

	FILE * invertedfp = fopen("invertedIndex.txt", "r");
	if (invertedfp == NULL)
		fprintf(stderr, "Error: invertedIndex.txt does not exist\n");

	char temp[100];
	char buffer[1000];
	fscanf(invertedfp, "%s", temp);
	while (strcmp(temp, term) != 0) {
		fscanf(invertedfp, "%s", temp);
		//printf("%s\n", temp);
	}

	int length = sizeof(buffer);

	// Move the file pointer forward by one to
	// counter the sneaky space
	// Trust me, it's necessary for what's going on here
	fseek(invertedfp, 1, SEEK_CUR);

	// Use fgets to get the whole line because it's easy and nice
	fgets(buffer, length, invertedfp);
	char buff = buffer[0];
	int pos = 0;
	//printf("Start\n");

	// Within the inner loop, start making a string for the url
	// so that we can parse it in and get tf at the same time!

	char dest[50];		// 50 seems bit enough for a url, right?
	int destpos = 0;

	// So I read the spec, and I don't actually need to parse in
	// per URL, so that's a thing...
	// I removed dest and destpos accordigly
	while (buff != '\n') {
		//printf("Outer loop\n");
		while (buff != ' ') {
			//printf("Inner loop %c\n", buff);
			dest[destpos] = buff;
			destpos++;
			pos++;
			buff = buffer[pos];
		}
		dest[destpos] = '\0';
		//printf("To test if string conversion worked %s\n", dest);

		if (!isElem(tallySet, dest))
			insertInto(tallySet, dest, 1.0);
		else {
			int place = nodeVal(tallySet, dest);
			// If not yet in the tally, place it in with count 1
			// Else replace by iterating the value
			int count = retrieveRank(tallySet, place);
			incrementRank(tallySet, place);
			insertInto(tallySet, dest, count + 1);
		}

		destpos = 0;
		//urlCount++;
		pos++;
		buff = buffer[pos];
	}

	return;
}

// Search the tree for terms, and output them in order
void outputTree(BSTree t, char * term) {
	// Recursively search the tree for a word term
	// Then go through the set and display the url's
	if (t == NULL) {
		return;		// This is if the term doesn't exist
	}
	// If less alphabetical, go left
	if (strcmp(term, t->value) < 0) {
		return outputTree(t->left, term);
	}
	// If more alphabetical, go right
	if (strcmp(term, t->value) > 0) {
		return outputTree(t->right, term);
	}

	// If none, then we found the term!
	// Print out the set inside
	// (Don't worry, it should already be pagerank ordered!)
	// AND THEY ACTUALLY ARE!
	int i = nElems(t->set);
	
	for (int k = 0; k < i; k++) {
		char * val = retrieveVal(t->set, k);
		float rank = retrieveRank(t->set, k);
		printf("---> %s, rank = %.7f\n", val, rank);
	}

	return;
}

// make a new node containing a value
static
BSTLink newBSTNode(char * v, char * url, Set pagerankSet)
{
	//printf("Making new node\n");
	BSTLink new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->value = strdup(v);
	new->left = new->right = NULL;
	new->set = newSet();
	int i = nodeVal(pagerankSet, url);
	float rank = retrieveRank(pagerankSet, i);
	insertInto(new->set, url, rank);
	return new;
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL) return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	disposeSet(t->set);
	free(t);
}

// display a BSTree
void showBSTree(BSTree t)
{
	//void doShowBSTree(BSTree); // see later
	//doShowBSTree(t);
	//printf("In show\n");
	if (t == NULL) {
		//printf("Found void as node\n");
		return;
	}
	if (t->left != NULL) {
		//printf("Recursing right\n");
		showBSTree(t->left);
	}
	//printf("Done recursing for now---------------\n");
	// Open the text file and write
	// Do this every time to be safe, as this is recursion
	FILE * fp = fopen("invertedIndex.txt", "a");
	fprintf(fp, "%s ", t->value);
	for (int i = 0; i < nElems(t->set); i++) {
		//printf("THIS IS TEMP LOOP\n");
		char * temp = retrieveVal(t->set, i);
		fprintf(fp, "%s ", temp);
	}
	fprintf(fp, "\n");
	fclose(fp);
	if (t->right != NULL) {
		//printf("Recursing left\n");
		showBSTree(t->right);
	}
}

// display BSTree root node
void showBSTreeNode(BSTree t)
{
	if (t == NULL) return;
	printf("%s ", t->value);
}

// print values in infix order
void BSTreeInfix(BSTree t)
{
	if (t == NULL) return;
	BSTreeInfix(t->left);
	showBSTreeNode(t);
	BSTreeInfix(t->right);
}

// print values in prefix order
void BSTreePrefix(BSTree t)
{
	if (t == NULL) return;
	showBSTreeNode(t);
	BSTreePrefix(t->left);
	BSTreePrefix(t->right);
}

// print values in postfix order
void BSTreePostfix(BSTree t)
{
	if (t == NULL) return;
	BSTreePostfix(t->left);
	BSTreePostfix(t->right);
	showBSTreeNode(t);
}

// print values in level-order
void BSTreeLevelOrder(BSTree t)
{
	//return; // 

	// My code here
	
  //      Queue q = newQueue();	    // Make a new queue
//	enterQueue(q, t);	    // Add the first tree node to the queue
        
	// Basic loop for as long as the queue has items to do
//	while (!QueueIsEmpty(q)) {
//	    BSTree node = QueueLeave(q);
//	    if (node == NULL)
//	       continue;
//	    printf("%d ", node->value);
//	    QueueJoin(q, node->left);
//	    QueueJoin(q, node->right);
//	}

	return;

	// End my code
}

// count #nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
	if (t == NULL)
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left)
	             + BSTreeNumNodes(t->right);
}

// count #leaves in BSTree
int BSTreeNumLeaves(BSTree t)
{
      	//return 0; // 
	if (t == NULL) 
	  return 0;
        //printf("Counting leaves --------> ");
	// My code here
	if (t->left == NULL && t->right == NULL) {
	  //printf("Found leaf at value %d\n", t->value);
	  return 1;
	}

	else {
	  //printf("Not at leaf with value\n");
	  return 0 + BSTreeNumLeaves(t->left)
	    +BSTreeNumLeaves(t->right);
	}
	// End my code
}

// insert a new value into a BSTree
// altered to  work with strings!
BSTree BSTreeInsert(BSTree t, char * v, char * url, Set pagerankSet)
{
	//printf("Value %s and url %s\n", v, url);
	if (t == NULL) {
		//printf("At null\n");
		//printf("New leaf\n");
		return newBSTNode(v, url, pagerankSet);
	}
	//printf("As of inserting, v is %s and t->value is %s -> ", v, t->value);
	//printf("For comparing %s and %s, strcmp says %d\n", v, t->value, strcmp(v, t->value));
	if (strcmp(v, t->value) < 0) {
		//printf("Less alphabetical\n");
		t->left = BSTreeInsert(t->left, v, url, pagerankSet);
	}
	else if (strcmp(v, t->value) > 0) {
		//printf("More alphabetical\n");
		t->right = BSTreeInsert(t->right, v, url, pagerankSet);
	}
	else {
		//printf("*********Equal value?\n");
		int i = nodeVal(pagerankSet, url);
		//printf("Received nodeVal of %d, and rank %f", i, retrieveRank(pagerankSet, i));
		float rank;

		if (i == -1)
			rank = 0;
		else
			rank = retrieveRank(pagerankSet, i);

		//printf("Just before inserting %s have %f\n", v, rank);
		insertInto(t->set, url, rank);
		// Put it into the set of the node
	}
		/* don't insert duplicates */;
	return t;
}

// check whether a value is in a BSTree
BSTree BSTreeFind(BSTree t, char * v)
{
	if (t == NULL)
		return 0;
	else if (strcmp(v, t->value) < 0)
		return BSTreeFind(t->left, v);
	else if (strcmp(v, t->value) > 0)
		return BSTreeFind(t->right, v);
	else // (v == t->value)
		return t;
}

// delete root of tree
static
BSTree deleteRoot(BSTree t)
{
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		free(t);
		return t->right;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
		free(t);
		return t->left;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor
	// - move its value to root
	// - delete inorder successor node
	BSTLink parent = t;
	BSTLink succ = t->right; // not null!
	while (succ->left != NULL) {
		parent = succ;
		succ = succ->left;
	}
	t->value = succ->value;
	free(succ);
	if (parent == t)
		parent->right = NULL;
	else
		parent->left = NULL;
	return t;
}

// delete a value from a BSTree
BSTree BSTreeDelete(BSTree t, char * v)
{
	if (t == NULL)
		return NULL;
	else if (strcmp(v, t->value) < 0)
		t->left = BSTreeDelete(t->left, v);
	else if (strcmp(v, t->value) > 0)
		t->right = BSTreeDelete(t->right, v);
	else // (v == t->value)
		t = deleteRoot(t);
	return t;
}

// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct
{
	asciinode * left, * right;
	//length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	//-1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	//max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
void print_level(asciinode *node, int x, int level);
void compute_edge_lengths(asciinode *node);
void compute_lprofile(asciinode *node, int x, int y);
void compute_rprofile(asciinode *node, int x, int y);
asciinode *build_ascii_tree(BSTree t);
void free_ascii_tree(asciinode *node);

#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

#define MIN(X,Y)  (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) > (Y)) ? (X) : (Y))

int gap = 3;  // gap between left and right nodes

//used for printing next node in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

//prints ascii tree for given Tree structure
void doShowBSTree(BSTree t)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL) return;
	proot = build_ascii_tree(t);
	compute_edge_lengths(proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN(xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level(proot, -xmin, i);
		printf("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf("(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree(proot);
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	if (level == 0)
	{
		for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++)
			printf(" ");
		print_next += i;
		printf("%s", node->label);
		print_next += node->lablen;
	}
	else if (node->edge_length >= level)
	{
		if (node->left != NULL)
		{
		  for (i=0; i<(x-print_next-(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("/");
		  print_next++;
		}
		if (node->right != NULL)
		{
		  for (i=0; i<(x-print_next+(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("\\");
		  print_next++;
		}
	}
	else
	{
		print_level(node->left,
			    x-node->edge_length-1,
			    level-node->edge_length-1);
		print_level(node->right,
			    x+node->edge_length+1,
			    level-node->edge_length-1);
	}
}


//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL) return;
	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else
	{
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
		    		lprofile[i] = INFINITY;
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		delta = 4;
		for (i=0; i<hmin; i++) {
			int w = gap+1+rprofile[i]-lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		//If the node has two children of height 1, then we allow the
		//two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
		      (node->right != NULL && node->right->height == 1))
				&& delta > 4)
			delta--;
		node->edge_length = ((delta+1)/2) - 1;
	}

	//now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX(node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX(node->right->height + node->edge_length + 1, h);
	node->height = h;
}

asciinode *build_ascii_tree_recursive(BSTree t)
{
	asciinode * node;

	if (t == NULL) return NULL;
	node = malloc(sizeof(asciinode));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);
	if (node->left != NULL) node->left->parent_dir = -1;
	if (node->right != NULL) node->right->parent_dir = 1;
	sprintf(node->label, "%s", t->value);
	node->lablen = strlen(node->label);;
	return node;
}


//Copy the tree into the ascii node structre
asciinode *build_ascii_tree(BSTree t)
{
	asciinode *node;
	if (t == NULL) return NULL;
	node = build_ascii_tree_recursive(t);
	node->parent_dir = 0;
	return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
	if (node == NULL) return;
	free_ascii_tree(node->left);
	free_ascii_tree(node->right);
	free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
	if (node->left != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
			lprofile[y+i] = MIN(lprofile[y+i], x-i);
	}
	compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL) return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
	if (node->right != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
	  		rprofile[y+i] = MAX(rprofile[y+i], x+i);
	}
	compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}
