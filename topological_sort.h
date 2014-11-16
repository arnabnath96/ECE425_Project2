#ifndef TOPOLOGICAL_SORT_H
#define TOPOLOGICAL_SORT_H

#include "graph.h"
#include "stack.h"

Stack* topological_sort(NODE*, int);
void depthfirst_traversal(NODE*, char*, Stack*, int);

#endif // TOPOLOGICAL_SORT_H
