// A quick and dirty library file
// Created 10/11/17
// Last edited: 10/11/17

#include "graph.h"
#include "set.h"
//#include "pagerankFunc.c"

Set PageRank(float d, float diffPR, int maxIterations);
float WValue(Graph g, float * links, int src, int dest, Set set);