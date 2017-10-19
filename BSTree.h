// BSTree.h ... interface to binary search tree ADT

#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNode *BSTree;
#include "set.h"

// Accumulate our search terms and amount of 'hits'
void accumulateTerms(char *, Set);
// Output the set to stdout
void outputTree(BSTree, char *);
// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree
void showBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(BSTree);

// print values in infix order
void BSTreeInfix(BSTree);
// print values in prefix order
void BSTreePrefix(BSTree);
// print values in postfix order
void BSTreePostfix(BSTree);
// print values in level-order
void BSTreeLevelOrder(BSTree);

// count #nodes in BSTree
int BSTreeNumNodes(BSTree);
// count #leaves in BSTree
int BSTreeNumLeaves(BSTree);

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree, char *, char *, Set);
// check whether a value is in a BSTree
BSTree BSTreeFind(BSTree, char *);
// delete a value from a BSTree
BSTree BSTreeDelete(BSTree, char *);

#endif
