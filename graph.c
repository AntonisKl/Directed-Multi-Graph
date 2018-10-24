#include "graph.h"
#include "stack.h"
#include "utils.h"

char* allocName(char* name) {
    return (char*)malloc(sizeof(name) + 1);
}

// initializes Vertice
Vertice* initVertice(char* name) {
    Vertice* vertice = (Vertice*)malloc(sizeof(Vertice));
    vertice->visited = 0;
    vertice->firstEdge = NULL;
    vertice->lastEdge = NULL;
    vertice->prevVertice = NULL;
    vertice->nextVertice = NULL;
    vertice->name = allocName(name);
    strcpy(vertice->name, name);
    return vertice;
}

// initializes Edge
Edge* initEdge(char* name) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->prevEdge = NULL;
    edge->nextEdge = NULL;
    edge->name = allocName(name);
    strcpy(edge->name, name);
    return edge;
}

// initializes Graph
Graph* initGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));  // MUST BE FREED
    graph->verticesNum = 0;
    graph->edgesNum = 0;
    return graph;
}

// frees all Edges of Vertice recursively starting from a specific Edge
void freeEdge(Edge* edge) {
    if (edge->nextEdge != NULL)
        freeEdge(edge->nextEdge);
    free(edge->name);
    free(edge);
    return;
}

// frees all Vertices of Graph recursively starting from a specific Vertice
void freeVertice(Vertice* vertice) {
    if (vertice->nextVertice != NULL)
        freeVertice(vertice->nextVertice);
    if (vertice->firstEdge != NULL)
        freeEdge(vertice->firstEdge);
    free(vertice->name);
    free(vertice);
    return;
}

// frees Graph recursively
void destroyGraph(Graph* graph) {
    if (graph != NULL) {
        if (graph->verticesNum > 0) {
            freeVertice(graph->firstVertice);
        } else {
            printf("Graph is empty\n");
            return;
        }
        free(graph);
    }
    printf("Graph was destroyed successfully\n");
    return;
}

// resets receiving Edges of Vertice to non-visited
void resetVisitedReceivingEdges(Graph* graph, char* nameTo) {
    Vertice* curVertice = graph->firstVertice;
    char found = 0;
    while (curVertice != NULL) {
        Edge* curEdge = curVertice->firstEdge;
        while (curEdge != NULL) {
            if (strcmp(curEdge->name, nameTo) == 0)
                curEdge->visited = 0;
            curEdge = curEdge->nextEdge;
        }
        curVertice = curVertice->nextVertice;
    }
    return;
}

// prints graph on file or on screen
void printGraph(Graph* graph, char* out) {
    FILE* fp;
    if (strcmp(out, "stdout") != 0) {
        fp = fopen(out, "w");
        if (fp == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
    }

    Vertice* curVertice = graph->firstVertice;
    while (curVertice != NULL) {
        if (strcmp(out, "stdout") != 0)
            fprintf(fp, "|%s|\n", curVertice->name);
        else
            printf("|%s|\n", curVertice->name);

        Edge* curEdge = curVertice->firstEdge;

        while (curEdge != NULL) {
            if (strcmp(out, "stdout") != 0)
                fprintf(fp, "\t%u->|%s|\n", curEdge->weight, curEdge->name);
            else
                printf("\t%u->|%s|\n", curEdge->weight, curEdge->name);

            curEdge = curEdge->nextEdge;
        }

        curVertice = curVertice->nextVertice;
    }
    if (strcmp(out, "stdout") == 0)
        printf("\n");

    if (strcmp(out, "stdout") != 0) {
        fclose(fp);
    }
    return;
}

// prints receiving Edges of Vertice
void printReceivingEdges(Graph* graph, char* nameTo) {
    Vertice* curVertice = graph->firstVertice;
    char found = 0;
    while (curVertice != NULL) {
        Edge* curEdge = curVertice->firstEdge;
        while (curEdge != NULL) {
            if (strcmp(curEdge->name, nameTo) == 0) {
                if (found == 0) {
                    printf("Rec-edges ");
                    printf("|%s|->%u->|%s|\n", curVertice->name, curEdge->weight, curEdge->name);
                    found = 1;
                } else
                    printf("          |%s|->%u->|%s|\n", curVertice->name, curEdge->weight, curEdge->name);
            }

            curEdge = curEdge->nextEdge;
        }

        curVertice = curVertice->nextVertice;
    }

    if (found == 0)
        printf("No-rec-edges %s\n\n", nameTo);
    else
        printf("\n");
}

// prints all simple circles starting from a specific Vertice
void printSimpleCirclesOfVertice(Graph* graph, char* name) {
    Vertice* startingVertice = findVerticeInGraph(graph, name);

    if (startingVertice == NULL) {
        printf("|%s| does not exist - abort-c;\n\n", name);
        return;
    }

    if (graph->verticesNum == 0) {
        return;
    }

    Stack* stack = initStack(graph->edgesNum);
    StackNode* poppedStackNodes[graph->edgesNum];
    unsigned int i = 0;
    for (i = 0; i < graph->edgesNum; i++)
        poppedStackNodes[i] = NULL;

    pushToStack(stack, startingVertice, 0, NULL);

    char simpleCircleFound = 0;
    i = 0;

    Edge* curEdge;
    // do DFS and don't visit already visited Vertices (Simple Circles)
    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);

        if (poppedStackNodes[i]->verticeFrom != NULL) {
            // mark edge as visited
            Edge* edge = findEdgeOfVertice(graph, poppedStackNodes[i]->verticeFrom, poppedStackNodes[i]->vertice->name, poppedStackNodes[i]->weight);
            edge->visited = 1;
        }

        if (poppedStackNodes[i]->vertice->visited == 0) {
            findVerticeInGraph(graph, poppedStackNodes[i]->vertice->name)->visited = 1;

            curEdge = poppedStackNodes[i]->vertice->firstEdge;
            if (curEdge != NULL) {
                while (curEdge != NULL) {
                    // push the non-visited Edges to the stack
                    if (curEdge->visited == 0)
                        pushToStack(stack, findVerticeInGraph(graph, curEdge->name), curEdge->weight, poppedStackNodes[i]->vertice);

                    curEdge = curEdge->nextEdge;
                }
                i++;
            } else {
                freeStackNode(poppedStackNodes[i]);
                poppedStackNodes[i] = NULL;
            }
        } else {
            if (strcmp(poppedStackNodes[i]->vertice->name, name) == 0) {
                unsigned int size = i;
                PathNode reversedPath[size], *correctPath[size];  // reversedPath is the path in the reverse order and DOES NOT include the current poppedStackNode

                char* curNameFrom = poppedStackNodes[i]->verticeFrom->name;
                unsigned int k = 0;

                // create reversed path according to the current nameFrom value
                for (int j = size - 1; j >= 0; j--) {
                    if (strcmp(poppedStackNodes[j]->vertice->name, curNameFrom) == 0) {
                        reversedPath[k].name = allocName(poppedStackNodes[j]->vertice->name);
                        strcpy(reversedPath[k].name, poppedStackNodes[j]->vertice->name);
                        if (poppedStackNodes[j]->verticeFrom != NULL) {
                            reversedPath[k].nameFrom = allocName(poppedStackNodes[j]->verticeFrom->name);
                            strcpy(reversedPath[k].nameFrom, poppedStackNodes[j]->verticeFrom->name);
                        } else
                            reversedPath[k].nameFrom = NULL;
                        reversedPath[k].weight = poppedStackNodes[j]->weight;
                        if (poppedStackNodes[j]->verticeFrom != NULL)
                            curNameFrom = poppedStackNodes[j]->verticeFrom->name;
                        k++;
                    }
                }

                // create correct order path
                unsigned int actualSize = k;
                for (int k = 0; k < actualSize; k++)
                    correctPath[k] = &reversedPath[actualSize - 1 - k];

                printf("\n\nCir-found ");
                printf("|%s|->", correctPath[0]->name);
                for (k = 1; k < actualSize; k++) {
                    printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                }
                // correctPath and reversedPath do not include the current popped stack node so print it manually
                printf("%u->|%s|\n\n\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->vertice->name);

                // free the path
                for (int k = 0; k < actualSize; k++) {
                    free(reversedPath[k].name);
                    reversedPath[k].name = NULL;
                    if (reversedPath[k].nameFrom != NULL) {
                        free(reversedPath[k].nameFrom);
                        reversedPath[k].nameFrom = NULL;
                    }
                }

                resetVisitedReceivingEdges(graph, name);

                // reset visited values for the next circle that will be found
                Vertice* curVertice = graph->firstVertice;
                while (curVertice != NULL) {
                    // RESET ONLY VERTICES
                    if (strcmp(curVertice->name, name) != 0)
                        curVertice->visited = 0;
                    curVertice = curVertice->nextVertice;
                }
                // at least one circle is found
                if (simpleCircleFound == 0)
                    simpleCircleFound = 1;
            }
        }
    }

    if (simpleCircleFound == 0)
        printf("No-circle-found |%s|\n\n", name);

    // free all popped stack nodes that have been malloced
    for (i = 0; i < graph->edgesNum; i++) {
        if (poppedStackNodes[i] != NULL) {
            freeStackNode(poppedStackNodes[i]);
            poppedStackNodes[i] = NULL;
        } else
            break;
    }

    // reset visited values for the next circle search
    Vertice* curVertice = graph->firstVertice;
    while (curVertice != NULL) {
        Edge* curEdge = curVertice->firstEdge;
        while (curEdge != NULL) {
            curEdge->visited = 0;
            curEdge = curEdge->nextEdge;
        }
        curVertice->visited = 0;
        curVertice = curVertice->nextVertice;
    }

    // destroy the stack
    destroyStack(stack);
    stack = NULL;
}

// prints all circles starting from a specific Vertice
void printAllCirclesOfVertice(Graph* graph, char* name, unsigned int minWeight) {
    Vertice* startingVertice = findVerticeInGraph(graph, name);

    if (startingVertice == NULL) {
        printf("|%s| does not exist - abort-f;\n\n", name);
        return;
    }

    if (graph->verticesNum == 0) {
        return;
    }

    unsigned int size = 2 * graph->edgesNum;

    Stack* stack = initStack(size);
    StackNode* poppedStackNodes[size];
    unsigned int i = 0;
    for (i = 0; i < size; i++) {
        poppedStackNodes[i] = NULL;
    }

    pushToStack(stack, startingVertice, 0, NULL);

    char circleFound = 0;
    unsigned int skipTimes = 0;
    unsigned int visitedPathIndex = 0;
    i = 0;

    Edge* curEdge;
    PathNode reversedPath[size], *correctPath[size], **reversedPathsVisited[size];
    for (int j = 0; j < size; j++)
        reversedPathsVisited[j] = NULL;

    // do DFS and visit Vertices without taking into account their visited value (General Circles)
    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);

        if (poppedStackNodes[i]->verticeFrom != NULL) {
            // mark edge as visited
            Edge* edge = findEdgeOfVertice(graph, poppedStackNodes[i]->verticeFrom, poppedStackNodes[i]->vertice->name, poppedStackNodes[i]->weight);
            edge->visited = 1;
        }

        if (poppedStackNodes[i]->vertice->visited == 1 && strcmp(poppedStackNodes[i]->vertice->name, name) == 0) {
            unsigned int size1 = i;

            for (int k = 0; k < size; k++)
                correctPath[k] = NULL;

            unsigned int skipTimes = 0;
            unsigned int k = 1;
            while (k > 0) {
                char* curNameFrom = poppedStackNodes[i]->verticeFrom->name;

                k = 0;
                unsigned int skipCounter = skipTimes;  // skipCounter is for backtracking and finding all different circles in the current state of poppedStackNodes
                for (int j = size1 - 1; j >= 0; j--) {
                    if (strcmp(poppedStackNodes[j]->vertice->name, curNameFrom) == 0 && skipCounter == 0) {
                        reversedPath[k].name = allocName(poppedStackNodes[j]->vertice->name);
                        strcpy(reversedPath[k].name, poppedStackNodes[j]->vertice->name);
                        if (poppedStackNodes[j]->verticeFrom != NULL) {
                            reversedPath[k].nameFrom = allocName(poppedStackNodes[j]->verticeFrom->name);
                            strcpy(reversedPath[k].nameFrom, poppedStackNodes[j]->verticeFrom->name);
                        } else
                            reversedPath[k].nameFrom = NULL;
                        reversedPath[k].weight = poppedStackNodes[j]->weight;
                        if (poppedStackNodes[j]->verticeFrom != NULL)
                            curNameFrom = poppedStackNodes[j]->verticeFrom->name;
                        k++;
                    }
                    if (strcmp(poppedStackNodes[j]->vertice->name, curNameFrom) == 0 && skipCounter > 0)
                        skipCounter--;
                }

                // if path size is > 0
                if (k > 0) {
                    unsigned int actualSize = k;  // actual path size (without NULL values)

                    char same = 0;
                    // search to the already visited path's array to see if the current path is the same as at least one previous one
                    for (int k = 0; k < visitedPathIndex; k++) {
                        unsigned int sameCounter = 0;
                        PathNode** curVisitedReversedPath = reversedPathsVisited[k];

                        // do this operation manually for the 0th element because the current popped stack node is not included in the current path array
                        if (curVisitedReversedPath[0] != NULL) {
                            if (poppedStackNodes[i]->verticeFrom->name != NULL) {
                                if (strcmp(poppedStackNodes[i]->vertice->name, curVisitedReversedPath[0]->name) == 0 &&
                                    strcmp(poppedStackNodes[i]->verticeFrom->name, curVisitedReversedPath[0]->nameFrom) == 0 &&
                                    poppedStackNodes[i]->weight == curVisitedReversedPath[0]->weight) {
                                    sameCounter++;
                                }
                            } else {
                                if (strcmp(poppedStackNodes[i]->vertice->name, curVisitedReversedPath[0]->name) == 0 &&
                                    poppedStackNodes[i]->weight == curVisitedReversedPath[0]->weight) {
                                    sameCounter++;
                                }
                            }
                        }

                        // continue for the rest of the elements
                        for (int j = 1; j < actualSize + 1; j++) {
                            if (curVisitedReversedPath[j] != NULL) {
                                if (reversedPath[j - 1].nameFrom != NULL) {
                                    if (strcmp(reversedPath[j - 1].name, curVisitedReversedPath[j]->name) == 0 &&
                                        strcmp(reversedPath[j - 1].nameFrom, curVisitedReversedPath[j]->nameFrom) == 0 &&
                                        reversedPath[j - 1].weight == curVisitedReversedPath[j]->weight) {
                                        sameCounter++;
                                    }
                                } else {
                                    if (strcmp(reversedPath[j - 1].name, curVisitedReversedPath[j]->name) == 0 &&
                                        reversedPath[j - 1].weight == curVisitedReversedPath[j]->weight) {
                                        sameCounter++;
                                    }
                                }
                            } else
                                break;
                        }

                        // found a same path so abort operation
                        if (sameCounter == actualSize + 1) {
                            same = 1;
                            break;
                        }
                    }

                    // if path is different or this is the first found path
                    if (same == 0 || visitedPathIndex == 0) {
                        reversedPathsVisited[visitedPathIndex] = (PathNode**)malloc(size * sizeof(PathNode));
                        for (int j = 0; j < size; j++)
                            reversedPathsVisited[visitedPathIndex][j] = NULL;

                        // store the path to the already visited paths' array
                        // do this operation manually for the 0th element because the current popped stack node is not included in the current path array
                        reversedPathsVisited[visitedPathIndex][0] = (PathNode*)malloc(sizeof(PathNode));

                        reversedPathsVisited[visitedPathIndex][0]->name = allocName(poppedStackNodes[i]->vertice->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->name, poppedStackNodes[i]->vertice->name);

                        reversedPathsVisited[visitedPathIndex][0]->nameFrom = allocName(poppedStackNodes[i]->verticeFrom->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->nameFrom, poppedStackNodes[i]->verticeFrom->name);

                        reversedPathsVisited[visitedPathIndex][0]->weight = poppedStackNodes[i]->weight;

                        // continue for the rest of the elements
                        for (int j = 1; j < actualSize + 1; j++) {
                            reversedPathsVisited[visitedPathIndex][j] = (PathNode*)malloc(sizeof(PathNode));

                            reversedPathsVisited[visitedPathIndex][j]->name = allocName(reversedPath[j - 1].name);
                            strcpy(reversedPathsVisited[visitedPathIndex][j]->name, reversedPath[j - 1].name);
                            if (reversedPath[j - 1].nameFrom != NULL) {
                                reversedPathsVisited[visitedPathIndex][j]->nameFrom = allocName(reversedPath[j - 1].nameFrom);
                                strcpy(reversedPathsVisited[visitedPathIndex][j]->nameFrom, reversedPath[j - 1].nameFrom);
                            } else
                                reversedPathsVisited[visitedPathIndex][j]->nameFrom = NULL;
                            reversedPathsVisited[visitedPathIndex][j]->weight = reversedPath[j - 1].weight;
                        }
                        visitedPathIndex++;

                        for (int k = 0; k < actualSize; k++)
                            correctPath[k] = &reversedPath[actualSize - 1 - k];

                        // print circle
                        printf("\n\nCir-found ");
                        printf("|%s|->", correctPath[0]->name);
                        for (int k = 1; k < actualSize; k++) {
                            printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                        }
                        printf("%u->|%s|\n\n\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->vertice->name);
                    }

                    for (int k = 0; k < size; k++) {
                        if (correctPath[k] != NULL) {
                            free(correctPath[k]->name);
                            correctPath[k]->name = NULL;
                            if (correctPath[k]->nameFrom != NULL) {
                                free(correctPath[k]->nameFrom);
                                correctPath[k]->nameFrom = NULL;
                            }
                            correctPath[k] = NULL;
                        } else
                            break;
                    }

                    // increment skipTimes because in the next found circle we want to backtrack farther
                    skipTimes++;
                }

                if (circleFound == 0)
                    circleFound = 1;
            }
            // reset visited values
            Vertice* curVertice = graph->firstVertice;
            while (curVertice != NULL) {
                // RESET ONLY VERTICES
                if (strcmp(curVertice->name, name) != 0)
                    curVertice->visited = 0;
                curVertice = curVertice->nextVertice;
            }

            resetVisitedReceivingEdges(graph, name);

        } else {
            findVerticeInGraph(graph, poppedStackNodes[i]->vertice->name)->visited = 1;

            curEdge = poppedStackNodes[i]->vertice->firstEdge;
            if (curEdge != NULL) {
                while (curEdge != NULL) {
                    if (curEdge->visited == 0 && curEdge->weight >= minWeight)
                        pushToStack(stack, findVerticeInGraph(graph, curEdge->name), curEdge->weight, poppedStackNodes[i]->vertice);

                    curEdge = curEdge->nextEdge;
                }
                i++;
            } else {
                // current node is useless
                freeStackNode(poppedStackNodes[i]);
                poppedStackNodes[i] = NULL;
            }
        }
    }

    if (circleFound == 0)
        printf("No-circle-found involving |%s| %u\n\n", name, minWeight);

    for (int i = 0; i < size; i++) {
        if (reversedPathsVisited[i] != NULL) {
            for (int j = 0; j < size; j++) {
                if (reversedPathsVisited[i][j] != NULL) {
                    free(reversedPathsVisited[i][j]->name);
                    reversedPathsVisited[i][j]->name = NULL;
                    if (reversedPathsVisited[i][j]->nameFrom != NULL) {
                        free(reversedPathsVisited[i][j]->nameFrom);
                        reversedPathsVisited[i][j]->nameFrom = NULL;
                    }
                    reversedPathsVisited[i][j] = NULL;
                }
            }
            reversedPathsVisited[i] = NULL;
        }
    }

    for (int i = 0; i < size; i++) {
        if (poppedStackNodes[i] != NULL) {
            freeStackNode(poppedStackNodes[i]);
            poppedStackNodes[i] = NULL;
        } else
            break;
    }

    // reset visited values
    Vertice* curVertice = graph->firstVertice;
    while (curVertice != NULL) {
        Edge* curEdge = curVertice->firstEdge;
        while (curEdge != NULL) {
            curEdge->visited = 0;
            curEdge = curEdge->nextEdge;
        }
        curVertice->visited = 0;
        curVertice = curVertice->nextVertice;
    }

    // destroy stack
    destroyStack(stack);
    stack = NULL;
}

Vertice* findVerticeInGraph(Graph* graph, char* name) {
    Vertice* curVertice = graph->firstVertice;
    while (curVertice != NULL) {
        if (strcmp(curVertice->name, name) == 0) {
            return curVertice;
        } else if (strcmp(name, curVertice->name) < 0)
            return NULL;

        curVertice = curVertice->nextVertice;
    }

    return NULL;
}

// finds 2 Vertices in Graph at once, it is more efficient than searching for them separately
void find2VerticesInGraph(Graph* graph, char* name1, char* name2, Vertice* foundVertices[2]) {
    Vertice *curVertice1 = graph->firstVertice, *curVertice2 = graph->firstVertice;
    foundVertices[0] = NULL;
    foundVertices[1] = NULL;

    char found1 = 0, found2 = 0, passed1 = 0, passed2 = 0;
    while (curVertice1 != NULL && curVertice2 != NULL) {
        if (found1 == 1) {
            if (strcmp(curVertice2->name, name2) == 0) {
                found2 = 1;
                foundVertices[1] = curVertice2;
                break;
            } else if (strcmp(name2, curVertice2->name) < 0)
                break;  // because list is sorted by name

            curVertice2 = curVertice2->nextVertice;
        } else if (found2 == 1) {
            if (strcmp(curVertice1->name, name1) == 0) {
                found1 = 1;
                foundVertices[0] = curVertice1;
                break;
            } else if (strcmp(name1, curVertice1->name) < 0)
                break;

            curVertice1 = curVertice1->nextVertice;
        } else {
            if (strcmp(curVertice1->name, name1) == 0) {
                found1 = 1;
                foundVertices[0] = curVertice1;
            } else if (strcmp(name1, curVertice1->name) < 0)
                passed1 = 1;  // actually vertice is not found, but set found1 = 1 just to speed up the process of search

            if (strcmp(curVertice2->name, name2) == 0) {
                found2 = 1;
                foundVertices[1] = curVertice2;
            } else if (strcmp(name2, curVertice2->name) < 0)
                passed2 = 1;  // actually vertice is not found, but set found2 = 1 just to speed up the process of search

            if (passed1 == 1 && passed2 == 1) {  // we passed the position where the vertices must have been
                                                 // (because list is sorted)
                foundVertices[0] = NULL;
                foundVertices[1] = NULL;
                return;
            }

            if (found1 == 0 || passed1 == 0)
                curVertice1 = curVertice1->nextVertice;

            if (found2 == 0 || passed2 == 0)
                curVertice2 = curVertice2->nextVertice;
        }
    }
    return;
}

// finds one Edge of a Vertice with or without weight comparison
Edge* findEdgeOfVertice(Graph* graph, Vertice* verticeFrom, char* nameTo, unsigned int weight) {
    char compareWeight = 1;
    if (weight == -1) {
        compareWeight = 0;
    }

    Edge* curEdge = verticeFrom->firstEdge;
    while (curEdge != NULL) {
        if (strcmp(nameTo, curEdge->name) == 0) {
            if (compareWeight == 1) {
                if (curEdge->weight == weight)
                    return curEdge;
            } else
                return curEdge;
        } else if (strcmp(nameTo, curEdge->name) < 0)
            return NULL;

        curEdge = curEdge->nextEdge;
    }

    return NULL;
}

// finds and deletes all Edges in graph that go to the Vertice with name: nameTo
void findAndDeleteEdgesInGraph(Graph* graph, char* nameTo) {
    Vertice* curVertice = graph->firstVertice;

    while (curVertice != NULL) {
        Edge* foundEdge = findEdgeOfVertice(graph, curVertice, nameTo, -1);

        while (foundEdge != NULL) {
            if (foundEdge->prevEdge != NULL) {
                if (foundEdge->nextEdge != NULL)
                    foundEdge->prevEdge->nextEdge = foundEdge->nextEdge;
                else {  // is last edge
                    foundEdge->prevEdge->nextEdge = NULL;
                    curVertice->lastEdge = foundEdge->prevEdge;
                }
            } else {
                if (foundEdge->nextEdge != NULL) {  // is first vertice
                    curVertice->firstEdge = foundEdge->nextEdge;
                    curVertice->firstEdge->prevEdge = NULL;
                } else {  // is the only edge
                    curVertice->firstEdge = NULL;
                    curVertice->lastEdge = NULL;
                }
            }

            free(foundEdge->name);
            free(foundEdge);

            curVertice->edgesNum--;
            graph->edgesNum--;

            foundEdge = findEdgeOfVertice(graph, curVertice, nameTo, -1);
        }

        curVertice = curVertice->nextVertice;
    }
}

// adds an Edge to a Vertice
void addEdgeToVertice(Vertice* vertice, char* name, unsigned int weight) {
    if (vertice->edgesNum == 0) {
        vertice->firstEdge = initEdge(name);
        vertice->firstEdge->weight = weight;

        vertice->lastEdge = vertice->firstEdge;
        vertice->edgesNum++;
        return;
    } else {
        Edge* curEdge = vertice->firstEdge;

        if (strcmp(name, curEdge->name) < 0) {
            Edge* edgeToInsert = initEdge(name);
            edgeToInsert->nextEdge = curEdge;
            edgeToInsert->weight = weight;

            curEdge->prevEdge = edgeToInsert;
            vertice->firstEdge = edgeToInsert;
            vertice->edgesNum++;
            return;
        } else {
            // search for a position in the sorted list
            while (curEdge != NULL) {
                if (curEdge->nextEdge != NULL) {
                    if (strcmp(name, curEdge->nextEdge->name) < 0) {
                        Edge* edgeToInsert = initEdge(name);
                        edgeToInsert->prevEdge = curEdge;
                        edgeToInsert->nextEdge = curEdge->nextEdge;
                        edgeToInsert->weight = weight;

                        curEdge->nextEdge->prevEdge = edgeToInsert;
                        curEdge->nextEdge = edgeToInsert;
                        vertice->edgesNum++;
                        return;
                    }
                } else {
                    vertice->lastEdge->nextEdge = initEdge(name);
                    vertice->lastEdge->nextEdge->prevEdge = curEdge;
                    vertice->lastEdge->nextEdge->weight = weight;

                    vertice->lastEdge = vertice->lastEdge->nextEdge;
                    vertice->edgesNum++;
                    return;
                }
                curEdge = curEdge->nextEdge;
            }
        }
    }
}

// inserts a Vertice to the Graph
Vertice* insertVerticeToGraph(Graph* graph, char* name) {
    Vertice* foundVertice = findVerticeInGraph(graph, name);

    if (foundVertice != NULL) {
        printf("Node |%s| Exists;\n\n", name);
        return NULL;
    }

    if (graph->verticesNum == 0) {
        graph->firstVertice = initVertice(name);

        graph->lastVertice = graph->firstVertice;
        graph->verticesNum++;
        printf("Inserted |%s|\n\n", name);
        return graph->firstVertice;
    } else {
        Vertice* curVertice = graph->firstVertice;
        // insert at start

        if (strcmp(name, curVertice->name) < 0) {
            Vertice* verticeToInsert = initVertice(name);
            verticeToInsert->nextVertice = curVertice;

            curVertice->prevVertice = verticeToInsert;
            graph->firstVertice = verticeToInsert;
            graph->verticesNum++;
            printf("Inserted |%s|\n\n", name);
            return graph->firstVertice;
        } else {
            while (curVertice != NULL) {
                if (curVertice->nextVertice != NULL) {
                    if (strcmp(name, curVertice->nextVertice->name) < 0) {
                        Vertice* verticeToInsert = initVertice(name);
                        verticeToInsert->prevVertice = curVertice;
                        verticeToInsert->nextVertice = curVertice->nextVertice;

                        curVertice->nextVertice->prevVertice = verticeToInsert;
                        curVertice->nextVertice = verticeToInsert;
                        graph->verticesNum++;
                        printf("Inserted |%s|\n\n", name);

                        return curVertice->nextVertice;
                    }
                } else {
                    graph->lastVertice->nextVertice = initVertice(name);
                    graph->lastVertice->nextVertice->prevVertice = curVertice;

                    graph->lastVertice = graph->lastVertice->nextVertice;
                    graph->verticesNum++;
                    printf("Inserted |%s|\n\n", name);
                    return graph->lastVertice;
                }

                curVertice = curVertice->nextVertice;
            }
        }
    }
}

// inserts an Edge to the Graph
void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight) {
    Vertice* foundVertices[2];
    Vertice* verticeTarget;
    if (strcmp(name1, name2) == 0) {
        verticeTarget = findVerticeInGraph(graph, name1);
        if (verticeTarget == NULL)
            verticeTarget = insertVerticeToGraph(graph, name1);
    } else {
        find2VerticesInGraph(graph, name1, name2, foundVertices);
        verticeTarget = foundVertices[0];
        if (foundVertices[0] == NULL)
            verticeTarget = insertVerticeToGraph(graph, name1);

        if (foundVertices[1] == NULL)
            insertVerticeToGraph(graph, name2);
    }

    addEdgeToVertice(verticeTarget, name2, weight);
    graph->edgesNum++;
    printf("Inserted |%s|->%u->|%s|\n\n", name1, weight, name2);
    return;
}

// deletes a Vertice from the Graph
void deleteVerticeFromGraph(Graph* graph, char* name) {
    Vertice* verticeToDelete = findVerticeInGraph(graph, name);
    if (verticeToDelete != NULL) {
        findAndDeleteEdgesInGraph(graph, verticeToDelete->name);

        if (verticeToDelete->firstEdge != NULL)
            freeEdge(verticeToDelete->firstEdge);

        if (verticeToDelete->prevVertice != NULL) {
            if (verticeToDelete->nextVertice != NULL) {
                verticeToDelete->prevVertice->nextVertice = verticeToDelete->nextVertice;
                verticeToDelete->nextVertice->prevVertice = verticeToDelete->prevVertice;
            } else {  // is last vertice
                verticeToDelete->prevVertice->nextVertice = NULL;
                graph->lastVertice = verticeToDelete->prevVertice;
            }
        } else {
            if (verticeToDelete->nextVertice != NULL) {  // is first vertice
                graph->firstVertice = verticeToDelete->nextVertice;
                graph->firstVertice->prevVertice = NULL;
            } else {  // is the only vertice
                graph->firstVertice = NULL;
                graph->lastVertice = NULL;
            }
        }
        free(verticeToDelete->name);
        free(verticeToDelete);
        verticeToDelete = NULL;

        graph->verticesNum--;

        printf("Deleted |%s|\n\n", name);
    } else
        printf("Node |%s| does not exist - abort-d;\n\n", name);

    return;
}

// deletes an Edge from the Graph
char deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weight) {
    Vertice* verticeFrom = findVerticeInGraph(graph, nameFrom);
    Vertice* verticeTo = findVerticeInGraph(graph, nameTo);

    if (verticeFrom == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameFrom);
        return 1;
    }

    if (verticeTo == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameTo);
        return 1;
    }

    Edge* edgeToDelete = findEdgeOfVertice(graph, verticeFrom, nameTo, weight);  // no weight comparison if weight == -1

    if (edgeToDelete == NULL) {
        printf("|%s|->%u->|%s| does not exist - abort-l;\n\n", nameFrom, weight, nameTo);
        return 1;
    }

    if (edgeToDelete->prevEdge != NULL) {
        if (edgeToDelete->nextEdge != NULL) {
            edgeToDelete->prevEdge->nextEdge = edgeToDelete->nextEdge;
            edgeToDelete->nextEdge->prevEdge = edgeToDelete->prevEdge;
        } else {  // is last conn vertice
            edgeToDelete->prevEdge->nextEdge = NULL;
            verticeFrom->lastEdge = edgeToDelete->prevEdge;
        }
    } else {
        if (edgeToDelete->nextEdge != NULL) {  // is first head vertice
            verticeFrom->firstEdge = edgeToDelete->nextEdge;
            verticeFrom->firstEdge->prevEdge = NULL;
        } else {  // is the only conn vertice
            verticeFrom->firstEdge = NULL;
            verticeFrom->lastEdge = NULL;
        }
    }

    free(edgeToDelete->name);
    free(edgeToDelete);
    edgeToDelete = NULL;

    verticeFrom->edgesNum--;
    printf("Del-vertex |%s|->%u->|%s|\n\n", nameFrom, weight, nameTo);
    return 0;
}

// modifies an existing Edge of the Graph
char modifyEdgeOfGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weightOld, unsigned int weightNew) {
    Vertice* verticeFrom = findVerticeInGraph(graph, nameFrom);
    Vertice* verticeTo = findVerticeInGraph(graph, nameTo);

    if (verticeFrom == NULL) {
        printf("|%s| does not exist - abort-m;\n\n", nameFrom);
        return 1;
    }

    if (verticeTo == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameTo);
        return 1;
    }

    Edge* edgeToModify = findEdgeOfVertice(graph, verticeFrom, nameTo, weightOld);

    if (edgeToModify == NULL) {
        printf("|%s|->%u->|%s| does not exist - abort-l;\n\n", nameFrom, weightOld, nameTo);
        return 1;
    }

    edgeToModify->weight = weightNew;
    printf("Mod-vertex |%s|->%u->|%s|\n\n", nameFrom, weightNew, nameTo);

    return 0;
}

// finds all paths between Vertices with names nameFrom and nameTo with max length pathLength
void traceFlow(Graph* graph, char* nameFrom, char* nameTo, unsigned int pathLength) {
    Vertice* startingVertice = findVerticeInGraph(graph, nameFrom);
    Vertice* endingVertice = findVerticeInGraph(graph, nameTo);

    if (startingVertice == NULL) {
        printf("|%s| does not exist - abort-t;\n\n", nameFrom);
        return;
    }

    if (endingVertice == NULL) {
        printf("|%s| does not exist - abort-t;\n\n", nameTo);
        return;
    }

    if (graph->verticesNum == 0) {
        return;
    }

    unsigned int size = 2 * graph->edgesNum;

    Stack* stack = initStack(size);
    StackNode* poppedStackNodes[size];
    unsigned int i = 0;
    for (i = 0; i < size; i++) {
        poppedStackNodes[i] = NULL;
    }

    pushToStack(stack, startingVertice, 0, NULL);

    char traceFound = 0;
    unsigned int skipTimes = 0;
    unsigned int visitedPathIndex = 0;
    i = 0;

    Edge* curEdge;
    PathNode reversedPath[size], *correctPath[size], **reversedPathsVisited[size];
    for (int j = 0; j < size; j++)
        reversedPathsVisited[j] = NULL;

    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);

        if (poppedStackNodes[i]->verticeFrom != NULL) {
            // mark edge as visited
            Edge* edge = findEdgeOfVertice(graph, poppedStackNodes[i]->verticeFrom, poppedStackNodes[i]->vertice->name, poppedStackNodes[i]->weight);
            edge->visited = 1;
        }
        printf("name: %s\n", poppedStackNodes[i]->vertice->name);

        if (strcmp(poppedStackNodes[i]->vertice->name, nameTo) == 0) {
            unsigned int size1 = i;

            for (int k = 0; k < size; k++)
                correctPath[k] = NULL;

            unsigned int skipTimes = 0;
            unsigned int k = 1;
            while (k > 0) {
                char* curNameFrom = poppedStackNodes[i]->verticeFrom->name;

                k = 0;
                unsigned int skipCounter = skipTimes;
                for (int j = size1 - 1; j >= 0; j--) {
                    if (strcmp(poppedStackNodes[j]->vertice->name, curNameFrom) == 0 && skipCounter == 0) {
                        reversedPath[k].name = allocName(poppedStackNodes[j]->vertice->name);
                        strcpy(reversedPath[k].name, poppedStackNodes[j]->vertice->name);
                        if (poppedStackNodes[j]->verticeFrom != NULL) {
                            reversedPath[k].nameFrom = allocName(poppedStackNodes[j]->verticeFrom->name);
                            strcpy(reversedPath[k].nameFrom, poppedStackNodes[j]->verticeFrom->name);
                        } else
                            reversedPath[k].nameFrom = NULL;
                        reversedPath[k].weight = poppedStackNodes[j]->weight;
                        if (poppedStackNodes[j]->verticeFrom != NULL)
                            curNameFrom = poppedStackNodes[j]->verticeFrom->name;
                        k++;
                    }
                    if (strcmp(poppedStackNodes[j]->vertice->name, curNameFrom) == 0 && skipCounter > 0)
                        skipCounter--;
                }

                if (k > 0) {
                    unsigned int actualSize = k;

                    char same = 0;
                    for (int k = 0; k < visitedPathIndex; k++) {
                        unsigned int sameCounter = 0;
                        PathNode** curVisitedReversedPath = reversedPathsVisited[k];

                        if (curVisitedReversedPath[0] != NULL) {
                            if (poppedStackNodes[i]->verticeFrom->name != NULL) {
                                if (strcmp(poppedStackNodes[i]->vertice->name, curVisitedReversedPath[0]->name) == 0 &&
                                    strcmp(poppedStackNodes[i]->verticeFrom->name, curVisitedReversedPath[0]->nameFrom) == 0 &&
                                    poppedStackNodes[i]->weight == curVisitedReversedPath[0]->weight) {
                                    sameCounter++;
                                }
                            } else {
                                if (strcmp(poppedStackNodes[i]->vertice->name, curVisitedReversedPath[0]->name) == 0 &&
                                    poppedStackNodes[i]->weight == curVisitedReversedPath[0]->weight) {
                                    sameCounter++;
                                }
                            }
                        }

                        for (int j = 1; j < actualSize + 1; j++) {
                            if (curVisitedReversedPath[j] != NULL) {
                                if (reversedPath[j - 1].nameFrom != NULL) {
                                    if (strcmp(reversedPath[j - 1].name, curVisitedReversedPath[j]->name) == 0 &&
                                        strcmp(reversedPath[j - 1].nameFrom, curVisitedReversedPath[j]->nameFrom) == 0 &&
                                        reversedPath[j - 1].weight == curVisitedReversedPath[j]->weight) {
                                        sameCounter++;
                                    }
                                } else {
                                    if (strcmp(reversedPath[j - 1].name, curVisitedReversedPath[j]->name) == 0 &&
                                        reversedPath[j - 1].weight == curVisitedReversedPath[j]->weight) {
                                        sameCounter++;
                                    }
                                }
                            } else
                                break;
                        }

                        if (sameCounter == actualSize + 1) {
                            same = 1;
                            break;
                        }
                    }

                    if (same == 0 || visitedPathIndex == 0) {
                        reversedPathsVisited[visitedPathIndex] = (PathNode**)malloc(size * sizeof(PathNode));
                        for (int j = 0; j < size; j++)
                            reversedPathsVisited[visitedPathIndex][j] = NULL;

                        reversedPathsVisited[visitedPathIndex][0] = (PathNode*)malloc(sizeof(PathNode));

                        reversedPathsVisited[visitedPathIndex][0]->name = allocName(poppedStackNodes[i]->vertice->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->name, poppedStackNodes[i]->vertice->name);

                        reversedPathsVisited[visitedPathIndex][0]->nameFrom = allocName(poppedStackNodes[i]->verticeFrom->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->nameFrom, poppedStackNodes[i]->verticeFrom->name);

                        reversedPathsVisited[visitedPathIndex][0]->weight = poppedStackNodes[i]->weight;

                        for (int j = 1; j < actualSize + 1; j++) {
                            reversedPathsVisited[visitedPathIndex][j] = (PathNode*)malloc(sizeof(PathNode));

                            reversedPathsVisited[visitedPathIndex][j]->name = allocName(reversedPath[j - 1].name);
                            strcpy(reversedPathsVisited[visitedPathIndex][j]->name, reversedPath[j - 1].name);
                            if (reversedPath[j - 1].nameFrom != NULL) {
                                reversedPathsVisited[visitedPathIndex][j]->nameFrom = allocName(reversedPath[j - 1].nameFrom);
                                strcpy(reversedPathsVisited[visitedPathIndex][j]->nameFrom, reversedPath[j - 1].nameFrom);
                            } else
                                reversedPathsVisited[visitedPathIndex][j]->nameFrom = NULL;
                            reversedPathsVisited[visitedPathIndex][j]->weight = reversedPath[j - 1].weight;
                        }

                        unsigned int edgesCounter = 0;
                        for (int j = 0; j < actualSize + 1; j++)
                            if (reversedPathsVisited[visitedPathIndex][j] != NULL)
                                edgesCounter++;

                        edgesCounter--;
                        visitedPathIndex++;

                        printf("path length: %u\n", pathLength);
                        if (edgesCounter <= pathLength) { // only if the count of Edges is <= max length print the current trace/path
                            for (int k = 0; k < actualSize; k++)
                                correctPath[k] = &reversedPath[actualSize - 1 - k];

                            printf("\n\nTra-found ");
                            printf("|%s|->", correctPath[0]->name);
                            for (int k = 1; k < actualSize; k++) {
                                printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                            }
                            printf("%u->|%s|\n\n\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->vertice->name);
                        }
                    }

                    for (int k = 0; k < size; k++) {
                        if (correctPath[k] != NULL) {
                            free(correctPath[k]->name);
                            correctPath[k]->name = NULL;
                            if (correctPath[k]->nameFrom != NULL) {
                                free(correctPath[k]->nameFrom);
                                correctPath[k]->nameFrom = NULL;
                            }
                            correctPath[k] = NULL;
                        } else
                            break;
                    }

                    skipTimes++;
                }

                if (traceFound == 0)
                    traceFound = 1;
            }
            // reset visited values
            Vertice* curVertice = graph->firstVertice;
            while (curVertice != NULL) {
                // RESET ONLY VERTICES
                if (strcmp(curVertice->name, nameFrom) != 0)
                    curVertice->visited = 0;
                curVertice = curVertice->nextVertice;
            }

            resetVisitedReceivingEdges(graph, nameFrom);

        } else {
            findVerticeInGraph(graph, poppedStackNodes[i]->vertice->name)->visited = 1;

            curEdge = poppedStackNodes[i]->vertice->firstEdge;
            if (curEdge != NULL) {
                while (curEdge != NULL) {
                    if (curEdge->visited == 0)
                        pushToStack(stack, findVerticeInGraph(graph, curEdge->name), curEdge->weight, poppedStackNodes[i]->vertice);

                    curEdge = curEdge->nextEdge;
                }
                i++;
            } else {
                freeStackNode(poppedStackNodes[i]);
                poppedStackNodes[i] = NULL;
            }
        }
    }

    if (traceFound == 0)
        printf("No-trace from |%s| to |%s|\n\n", nameFrom, nameTo);

    for (int i = 0; i < size; i++) {
        if (reversedPathsVisited[i] != NULL) {
            for (int j = 0; j < size; j++) {
                if (reversedPathsVisited[i][j] != NULL) {
                    free(reversedPathsVisited[i][j]->name);
                    reversedPathsVisited[i][j]->name = NULL;
                    if (reversedPathsVisited[i][j]->nameFrom != NULL) {
                        free(reversedPathsVisited[i][j]->nameFrom);
                        reversedPathsVisited[i][j]->nameFrom = NULL;
                    }
                    reversedPathsVisited[i][j] = NULL;
                }
            }
            reversedPathsVisited[i] = NULL;
        }
    }

    for (int i = 0; i < size; i++) {
        if (poppedStackNodes[i] != NULL) {
            freeStackNode(poppedStackNodes[i]);
            poppedStackNodes[i] = NULL;
        } else
            break;
    }

    // reset visited values
    Vertice* curVertice = graph->firstVertice;
    while (curVertice != NULL) {
        Edge* curEdge = curVertice->firstEdge;
        while (curEdge != NULL) {
            curEdge->visited = 0;
            curEdge = curEdge->nextEdge;
        }
        curVertice->visited = 0;
        curVertice = curVertice->nextVertice;
    }

    // destroy stack
    destroyStack(stack);
    stack = NULL;
}