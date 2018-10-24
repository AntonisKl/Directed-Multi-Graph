#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

typedef struct PathNode {
    char* nameFrom;
    char* name;
    unsigned int weight;
} PathNode;

void removeSpaces(char *str);

void handleFlags(int argc, char** argv, char** inputFileName, char** outputFileName);
char handleInput(char* input, Graph* graph);

#endif
