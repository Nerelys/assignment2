// A quick and dirty library file
// Created 10/11/17
// Last edited: 10/11/17

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "set.h"

Set PageRank(float d, float diffPR, int maxIterations);
float WValue(Graph g, float * links, int src, int dest, Set set);
