#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void removeSpaces(char *str);

void handleFlags(int argc, char** argv, char* inputFileName, char* outputFileName);
char handleInput(char* input, Graph* graph);

#endif
