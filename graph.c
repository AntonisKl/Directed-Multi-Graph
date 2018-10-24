#include "graph.h"
#include "stack.h"
#include "utils.h"

char* allocName(char* name) {
    return (char*)malloc(sizeof(name) + 1);
}

HeadVertice* initHeadVertice(char* name) {
    HeadVertice* headVertice = (HeadVertice*)malloc(sizeof(HeadVertice));
    headVertice->visited = 0;
    headVertice->firstConnVertice = NULL;
    headVertice->lastConnVertice = NULL;
    headVertice->prevHeadVertice = NULL;
    headVertice->nextHeadVertice = NULL;
    headVertice->name = allocName(name);
    strcpy(headVertice->name, name);
    return headVertice;
}

ConnVertice* initConnVertice(char* name) {
    ConnVertice* connVertice = (ConnVertice*)malloc(sizeof(ConnVertice));
    connVertice->prevConnVertice = NULL;
    connVertice->nextConnVertice = NULL;
    connVertice->name = allocName(name);
    strcpy(connVertice->name, name);
    return connVertice;
}

Graph* initGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));  // MUST BE FREED
    graph->verticesNum = 0;
    graph->edgesNum = 0;
    return graph;
}

void freeConnVertice(ConnVertice* connVertice) {
    if (connVertice->nextConnVertice != NULL)
        freeConnVertice(connVertice->nextConnVertice);
    free(connVertice->name);
    free(connVertice);
    return;
}

void freeHeadVertice(HeadVertice* headVertice) {
    if (headVertice->nextHeadVertice != NULL)
        freeHeadVertice(headVertice->nextHeadVertice);
    if (headVertice->firstConnVertice != NULL)
        freeConnVertice(headVertice->firstConnVertice);
    free(headVertice->name);
    free(headVertice);
    return;
}

void destroyGraph(Graph* graph) {
    if (graph != NULL) {
        if (graph->verticesNum > 0) {
            freeHeadVertice(graph->firstVertice);
        } else {
            printf("Graph is empty\n");
            return;
        }
        free(graph);
    }
    printf("Graph was destroyed successfully\n");
    return;
}

void resetVisitedReceivingEdges(Graph* graph, char* nameTo) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    char found = 0;
    while (curHeadVertice != NULL) {
        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;
        while (curConnVertice != NULL) {
            if (strcmp(curConnVertice->name, nameTo) == 0)
                curConnVertice->visited = 0;
            curConnVertice = curConnVertice->nextConnVertice;
        }
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
    return;
}

void printGraph(Graph* graph, char* out) {
    FILE* fp;
    if (strcmp(out, "stdout") != 0) {
        fp = fopen(out, "w");
        if (fp == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
    }

    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        if (strcmp(out, "stdout") != 0)
            fprintf(fp, "|%s|\n", curHeadVertice->name);
        else
            printf("|%s|\n", curHeadVertice->name);

        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;

        while (curConnVertice != NULL) {
            if (strcmp(out, "stdout") != 0)
                fprintf(fp, "\t%u->|%s|\n", curConnVertice->weight, curConnVertice->name);
            else
                printf("\t%u->|%s|\n", curConnVertice->weight, curConnVertice->name);

            curConnVertice = curConnVertice->nextConnVertice;
        }

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
    if (strcmp(out, "stdout") == 0)
        printf("\n");

    if (strcmp(out, "stdout") != 0) {
        fclose(fp);
    }
    return;
}

void printReceivingEdges(Graph* graph, char* nameTo) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    char found = 0;
    while (curHeadVertice != NULL) {
        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;
        while (curConnVertice != NULL) {
            if (strcmp(curConnVertice->name, nameTo) == 0) {
                if (found == 0) {
                    printf("Rec-edges ");
                    printf("|%s|->%u->|%s|\n", curHeadVertice->name, curConnVertice->weight, curConnVertice->name);
                    found = 1;
                } else
                    printf("          |%s|->%u->|%s|\n", curHeadVertice->name, curConnVertice->weight, curConnVertice->name);
            }

            curConnVertice = curConnVertice->nextConnVertice;
        }

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }

    if (found == 0)
        printf("No-rec-edges %s\n\n", nameTo);
    else
        printf("\n");
}

void printSimpleCirclesOfNode(Graph* graph, char* name) {
    HeadVertice* startingHeadVertice = findHeadVerticeInGraph(graph, name);

    if (startingHeadVertice == NULL) {
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

    pushToStack(stack, startingHeadVertice, 0, NULL);

    char simpleCircleFound = 0;
    i = 0;

    ConnVertice* curConnVertice;
    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);

        if (poppedStackNodes[i]->headVerticeFrom != NULL) {
            // mark edge as visited
            ConnVertice* edge = findEdgeOfNode(graph, poppedStackNodes[i]->headVerticeFrom, poppedStackNodes[i]->headVertice->name, poppedStackNodes[i]->weight);
            edge->visited = 1;
        }

        if (poppedStackNodes[i]->headVertice->visited == 0) {
            findHeadVerticeInGraph(graph, poppedStackNodes[i]->headVertice->name)->visited = 1;

            curConnVertice = poppedStackNodes[i]->headVertice->firstConnVertice;
            if (curConnVertice != NULL) {
                while (curConnVertice != NULL) {
                    if (curConnVertice->visited == 0)
                        pushToStack(stack, findHeadVerticeInGraph(graph, curConnVertice->name), curConnVertice->weight, poppedStackNodes[i]->headVertice);

                    curConnVertice = curConnVertice->nextConnVertice;
                }
                i++;
            } else {
                freeStackNode(poppedStackNodes[i]);
                poppedStackNodes[i] = NULL;
            }
        } else {
            if (strcmp(poppedStackNodes[i]->headVertice->name, name) == 0) {
                unsigned int size = i;
                PathNode reversedPath[size], *correctPath[size];

                char* curNameFrom = poppedStackNodes[i]->headVerticeFrom->name;
                unsigned int k = 0;

                for (int j = size - 1; j >= 0; j--) {
                    if (strcmp(poppedStackNodes[j]->headVertice->name, curNameFrom) == 0) {
                        reversedPath[k].name = allocName(poppedStackNodes[j]->headVertice->name);
                        strcpy(reversedPath[k].name, poppedStackNodes[j]->headVertice->name);
                        if (poppedStackNodes[j]->headVerticeFrom != NULL) {
                            reversedPath[k].nameFrom = allocName(poppedStackNodes[j]->headVerticeFrom->name);
                            strcpy(reversedPath[k].nameFrom, poppedStackNodes[j]->headVerticeFrom->name);
                        } else
                            reversedPath[k].nameFrom = NULL;
                        reversedPath[k].weight = poppedStackNodes[j]->weight;
                        if (poppedStackNodes[j]->headVerticeFrom != NULL)
                            curNameFrom = poppedStackNodes[j]->headVerticeFrom->name;
                        k++;
                    }
                }

                unsigned int actualSize = k;
                for (int k = 0; k < actualSize; k++)
                    correctPath[k] = &reversedPath[actualSize - 1 - k];

                printf("\n\nCir-found ");
                printf("|%s|->", correctPath[0]->name);
                for (k = 1; k < actualSize; k++) {
                    printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                }
                printf("%u->|%s|\n\n\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->headVertice->name);

                for (int k = 0; k < actualSize; k++) {
                    free(reversedPath[k].name);
                    reversedPath[k].name = NULL;
                    if (reversedPath[k].nameFrom != NULL) {
                        free(reversedPath[k].nameFrom);
                        reversedPath[k].nameFrom = NULL;
                    }
                }

                resetVisitedReceivingEdges(graph, name);

                // reset visited values
                HeadVertice* curHeadVertice = graph->firstVertice;
                while (curHeadVertice != NULL) {
                    // ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;
                    // while (curConnVertice != NULL) {
                    //     curConnVertice->visited = 0;
                    //     curConnVertice = curConnVertice->nextConnVertice;
                    // }

                    // RESET ONLY VERTICES
                    if (strcmp(curHeadVertice->name, name) != 0)
                        curHeadVertice->visited = 0;
                    curHeadVertice = curHeadVertice->nextHeadVertice;
                }
                if (simpleCircleFound == 0)
                    simpleCircleFound = 1;
            }
        }
    }

    if (simpleCircleFound == 0)
        printf("No-circle-found |%s|\n\n", name);

    for (i = 0; i < graph->edgesNum; i++) {
        if (poppedStackNodes[i] != NULL) {
            freeStackNode(poppedStackNodes[i]);
            poppedStackNodes[i] = NULL;
        } else
            break;
    }

    // reset visited values
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;
        while (curConnVertice != NULL) {
            curConnVertice->visited = 0;
            curConnVertice = curConnVertice->nextConnVertice;
        }
        curHeadVertice->visited = 0;
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }

    destroyStack(stack);
}

void printAllCirclesOfNode(Graph* graph, char* name, unsigned int minWeight) {
    HeadVertice* startingHeadVertice = findHeadVerticeInGraph(graph, name);

    if (startingHeadVertice == NULL) {
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

    pushToStack(stack, startingHeadVertice, 0, NULL);

    char circleFound = 0;
    unsigned int skipTimes = 0;
    unsigned int visitedPathIndex = 0;
    i = 0;

    ConnVertice* curConnVertice;
    PathNode reversedPath[size], *correctPath[size], **reversedPathsVisited[size];
    for (int j = 0; j < size; j++)
        reversedPathsVisited[j] = NULL;

    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);

        if (poppedStackNodes[i]->headVerticeFrom != NULL) {
            // mark edge as visited
            ConnVertice* edge = findEdgeOfNode(graph, poppedStackNodes[i]->headVerticeFrom, poppedStackNodes[i]->headVertice->name, poppedStackNodes[i]->weight);
            edge->visited = 1;
        }

        if (poppedStackNodes[i]->headVertice->visited == 1 && strcmp(poppedStackNodes[i]->headVertice->name, name) == 0) {
            unsigned int size1 = i;

            for (int k = 0; k < size; k++)
                correctPath[k] = NULL;

            unsigned int skipTimes = 0;
            unsigned int k = 1;
            while (k > 0) {
                char* curNameFrom = poppedStackNodes[i]->headVerticeFrom->name;

                k = 0;
                unsigned int skipCounter = skipTimes;
                for (int j = size1 - 1; j >= 0; j--) {
                    if (strcmp(poppedStackNodes[j]->headVertice->name, curNameFrom) == 0 && skipCounter == 0) {
                        reversedPath[k].name = allocName(poppedStackNodes[j]->headVertice->name);
                        strcpy(reversedPath[k].name, poppedStackNodes[j]->headVertice->name);
                        if (poppedStackNodes[j]->headVerticeFrom != NULL) {
                            reversedPath[k].nameFrom = allocName(poppedStackNodes[j]->headVerticeFrom->name);
                            strcpy(reversedPath[k].nameFrom, poppedStackNodes[j]->headVerticeFrom->name);
                        } else
                            reversedPath[k].nameFrom = NULL;
                        reversedPath[k].weight = poppedStackNodes[j]->weight;
                        if (poppedStackNodes[j]->headVerticeFrom != NULL)
                            curNameFrom = poppedStackNodes[j]->headVerticeFrom->name;
                        k++;
                    }
                    if (strcmp(poppedStackNodes[j]->headVertice->name, curNameFrom) == 0 && skipCounter > 0)
                        skipCounter--;
                }

                if (k > 0) {
                    unsigned int actualSize = k;

                    char same = 0;
                    for (int k = 0; k < visitedPathIndex; k++) {
                        unsigned int sameCounter = 0;
                        PathNode** curVisitedReversedPath = reversedPathsVisited[k];

                        if (curVisitedReversedPath[0] != NULL) {
                            if (poppedStackNodes[i]->headVerticeFrom->name != NULL) {
                                if (strcmp(poppedStackNodes[i]->headVertice->name, curVisitedReversedPath[0]->name) == 0 &&
                                    strcmp(poppedStackNodes[i]->headVerticeFrom->name, curVisitedReversedPath[0]->nameFrom) == 0 &&
                                    poppedStackNodes[i]->weight == curVisitedReversedPath[0]->weight) {
                                    sameCounter++;
                                }
                            } else {
                                if (strcmp(poppedStackNodes[i]->headVertice->name, curVisitedReversedPath[0]->name) == 0 &&
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

                        reversedPathsVisited[visitedPathIndex][0]->name = allocName(poppedStackNodes[i]->headVertice->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->name, poppedStackNodes[i]->headVertice->name);

                        reversedPathsVisited[visitedPathIndex][0]->nameFrom = allocName(poppedStackNodes[i]->headVerticeFrom->name);
                        strcpy(reversedPathsVisited[visitedPathIndex][0]->nameFrom, poppedStackNodes[i]->headVerticeFrom->name);
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
                        visitedPathIndex++;

                        for (int k = 0; k < actualSize; k++)
                            correctPath[k] = &reversedPath[actualSize - 1 - k];

                        printf("\n\nCir-found ");
                        printf("|%s|->", correctPath[0]->name);
                        for (int k = 1; k < actualSize; k++) {
                            printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                        }
                        printf("%u->|%s|\n\n\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->headVertice->name);
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

                if (circleFound == 0)
                    circleFound = 1;
            }
            // reset visited values
            HeadVertice* curHeadVertice = graph->firstVertice;
            while (curHeadVertice != NULL) {
                // RESET ONLY VERTICES
                if (strcmp(curHeadVertice->name, name) != 0)
                    curHeadVertice->visited = 0;
                curHeadVertice = curHeadVertice->nextHeadVertice;
            }

            resetVisitedReceivingEdges(graph, name);

        } else {
            findHeadVerticeInGraph(graph, poppedStackNodes[i]->headVertice->name)->visited = 1;

            curConnVertice = poppedStackNodes[i]->headVertice->firstConnVertice;
            if (curConnVertice != NULL) {
                while (curConnVertice != NULL) {
                    if (curConnVertice->visited == 0 && curConnVertice->weight >= minWeight)
                        pushToStack(stack, findHeadVerticeInGraph(graph, curConnVertice->name), curConnVertice->weight, poppedStackNodes[i]->headVertice);

                    curConnVertice = curConnVertice->nextConnVertice;
                }
                i++;
            } else {
                freeStackNode(poppedStackNodes[i]);
                poppedStackNodes[i] = NULL;
            }
        }
    }

    if (circleFound == 0)
        printf("No-circle-found involving |%s| %u \n", name, minWeight);

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
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;
        while (curConnVertice != NULL) {
            curConnVertice->visited = 0;
            curConnVertice = curConnVertice->nextConnVertice;
        }
        curHeadVertice->visited = 0;
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }

    // destroy stack
    destroyStack(stack);
    stack = NULL;
}

HeadVertice* findHeadVerticeInGraph(Graph* graph, char* name) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        if (strcmp(curHeadVertice->name, name) == 0) {
            return curHeadVertice;
        } else if (strcmp(name, curHeadVertice->name) < 0)
            return NULL;

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }

    return NULL;
}

void find2HeadVerticesInGraph(Graph* graph, char* name1, char* name2, HeadVertice* foundHeadVertices[2]) {
    HeadVertice *curHeadVertice1 = graph->firstVertice, *curHeadVertice2 = graph->firstVertice;
    foundHeadVertices[0] = NULL;
    foundHeadVertices[1] = NULL;

    char found1 = 0, found2 = 0, passed1 = 0, passed2 = 0;
    while (curHeadVertice1 != NULL && curHeadVertice2 != NULL) {
        if (found1 == 1) {
            if (strcmp(curHeadVertice2->name, name2) == 0) {
                found2 = 1;
                foundHeadVertices[1] = curHeadVertice2;
                break;
            } else if (strcmp(name2, curHeadVertice2->name) < 0)
                break;  // because list is sorted by name

            curHeadVertice2 = curHeadVertice2->nextHeadVertice;
        } else if (found2 == 1) {
            if (strcmp(curHeadVertice1->name, name1) == 0) {
                found1 = 1;
                foundHeadVertices[0] = curHeadVertice1;
                break;
            } else if (strcmp(name1, curHeadVertice1->name) < 0)
                break;

            curHeadVertice1 = curHeadVertice1->nextHeadVertice;
        } else {
            if (strcmp(curHeadVertice1->name, name1) == 0) {
                found1 = 1;
                foundHeadVertices[0] = curHeadVertice1;
            } else if (strcmp(name1, curHeadVertice1->name) < 0)
                passed1 = 1;  // actually vertice is not found, but set found1 = 1 just to speed up the process of search

            if (strcmp(curHeadVertice2->name, name2) == 0) {
                found2 = 1;
                foundHeadVertices[1] = curHeadVertice2;
            } else if (strcmp(name2, curHeadVertice2->name) < 0)
                passed2 = 1;  // actually vertice is not found, but set found2 = 1 just to speed up the process of search

            if (passed1 == 1 && passed2 == 1) {  // we passed the position where the vertices must have been
                                                 // (because list is sorted)
                foundHeadVertices[0] = NULL;
                foundHeadVertices[1] = NULL;
                return;
            }

            if (found1 == 0 || passed1 == 0)
                curHeadVertice1 = curHeadVertice1->nextHeadVertice;

            if (found2 == 0 || passed2 == 0)
                curHeadVertice2 = curHeadVertice2->nextHeadVertice;
        }
    }
    return;
}

ConnVertice* findEdgeOfNode(Graph* graph, HeadVertice* headVerticeFrom, char* nameTo, unsigned int weight) {
    char compareWeight = 1;
    if (weight == -1) {
        compareWeight = 0;
    }

    ConnVertice* curConnVertice = headVerticeFrom->firstConnVertice;
    while (curConnVertice != NULL) {
        if (strcmp(nameTo, curConnVertice->name) == 0) {
            if (compareWeight == 1) {
                if (curConnVertice->weight == weight)
                    return curConnVertice;
            } else
                return curConnVertice;
        } else if (strcmp(nameTo, curConnVertice->name) < 0)
            return NULL;

        curConnVertice = curConnVertice->nextConnVertice;
    }

    return NULL;
}

void findAndDeleteEdgesInGraph(Graph* graph, char* nameTo) {
    HeadVertice* curHeadVertice = graph->firstVertice;

    while (curHeadVertice != NULL) {
        ConnVertice* foundEdge = findEdgeOfNode(graph, curHeadVertice, nameTo, -1);

        while (foundEdge != NULL) {
            if (foundEdge->prevConnVertice != NULL) {
                if (foundEdge->nextConnVertice != NULL)
                    foundEdge->prevConnVertice->nextConnVertice = foundEdge->nextConnVertice;
                else {  // is last conn vertice
                    foundEdge->prevConnVertice->nextConnVertice = NULL;
                    curHeadVertice->lastConnVertice = foundEdge->prevConnVertice;
                }
            } else {
                if (foundEdge->nextConnVertice != NULL) {  // is first head vertice
                    curHeadVertice->firstConnVertice = foundEdge->nextConnVertice;
                    curHeadVertice->firstConnVertice->prevConnVertice = NULL;
                } else {  // is the only conn vertice
                    curHeadVertice->firstConnVertice = NULL;
                    curHeadVertice->lastConnVertice = NULL;
                }
            }

            free(foundEdge->name);
            // connVerticeToDelete->nextConnVertice = NULL;
            free(foundEdge);

            curHeadVertice->connVerticesNum--;
            graph->edgesNum--;

            foundEdge = findEdgeOfNode(graph, curHeadVertice, nameTo, -1);
        }

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
}

void addConnVerticeToHeadVertice(HeadVertice* headVertice, char* name, unsigned int weight) {
    if (headVertice->connVerticesNum == 0) {
        headVertice->firstConnVertice = initConnVertice(name);
        headVertice->firstConnVertice->weight = weight;

        headVertice->lastConnVertice = headVertice->firstConnVertice;
        headVertice->connVerticesNum++;
        return;
    } else {
        ConnVertice* curConnVertice = headVertice->firstConnVertice;

        if (strcmp(name, curConnVertice->name) < 0) {
            ConnVertice* connVerticeToInsert = initConnVertice(name);
            connVerticeToInsert->nextConnVertice = curConnVertice;
            connVerticeToInsert->weight = weight;

            curConnVertice->prevConnVertice = connVerticeToInsert;
            headVertice->firstConnVertice = connVerticeToInsert;
            headVertice->connVerticesNum++;
            return;
        } else {
            // search for a position in the sorted list
            while (curConnVertice != NULL) {
                if (curConnVertice->nextConnVertice != NULL) {
                    if (strcmp(name, curConnVertice->nextConnVertice->name) < 0) {
                        ConnVertice* connVerticeToInsert = initConnVertice(name);
                        connVerticeToInsert->prevConnVertice = curConnVertice;
                        connVerticeToInsert->nextConnVertice = curConnVertice->nextConnVertice;
                        connVerticeToInsert->weight = weight;

                        curConnVertice->nextConnVertice->prevConnVertice = connVerticeToInsert;
                        curConnVertice->nextConnVertice = connVerticeToInsert;
                        headVertice->connVerticesNum++;
                        return;
                    }
                } else {
                    headVertice->lastConnVertice->nextConnVertice = initConnVertice(name);
                    headVertice->lastConnVertice->nextConnVertice->prevConnVertice = curConnVertice;
                    headVertice->lastConnVertice->nextConnVertice->weight = weight;

                    headVertice->lastConnVertice = headVertice->lastConnVertice->nextConnVertice;
                    headVertice->connVerticesNum++;
                    return;
                }
                curConnVertice = curConnVertice->nextConnVertice;
            }
        }
        // connVerticeToBeCreated = headVertice->lastConnVertice->nextConnVertice;

        // headVertice->lastConnVertice->nextConnVertice = allocConnVertice();
        // headVertice->lastConnVertice->nextConnVertice->name = allocName(name);
        // strcpy(headVertice->lastConnVertice->nextConnVertice->name, name);
        // headVertice->lastConnVertice->nextConnVertice->weight = weight;

        // headVertice->lastConnVertice =
        //     headVertice->lastConnVertice->nextConnVertice;
    }
}

HeadVertice* insertVerticeToGraph(Graph* graph, char* name) {
    HeadVertice* foundHeadVertice = findHeadVerticeInGraph(graph, name);

    if (foundHeadVertice != NULL) {
        printf("Node |%s| Exists;\n\n", name);
        return NULL;
    }

    if (graph->verticesNum == 0) {
        graph->firstVertice = initHeadVertice(name);

        graph->lastVertice = graph->firstVertice;
        graph->verticesNum++;
        printf("Inserted |%s|\n\n", name);
        return graph->firstVertice;
    } else {
        HeadVertice* curHeadVertice = graph->firstVertice;
        // insert at start

        if (strcmp(name, curHeadVertice->name) < 0) {
            HeadVertice* headVerticeToInsert = initHeadVertice(name);
            headVerticeToInsert->nextHeadVertice = curHeadVertice;

            curHeadVertice->prevHeadVertice = headVerticeToInsert;
            graph->firstVertice = headVerticeToInsert;
            graph->verticesNum++;
            printf("Inserted |%s|\n\n", name);
            return graph->firstVertice;
        } else {
            while (curHeadVertice != NULL) {
                if (curHeadVertice->nextHeadVertice != NULL) {
                    if (strcmp(name, curHeadVertice->nextHeadVertice->name) < 0) {
                        HeadVertice* headVerticeToInsert = initHeadVertice(name);
                        headVerticeToInsert->prevHeadVertice = curHeadVertice;
                        headVerticeToInsert->nextHeadVertice = curHeadVertice->nextHeadVertice;

                        curHeadVertice->nextHeadVertice->prevHeadVertice = headVerticeToInsert;
                        curHeadVertice->nextHeadVertice = headVerticeToInsert;
                        graph->verticesNum++;
                        printf("Inserted |%s|\n\n", name);

                        return curHeadVertice->nextHeadVertice;
                    }
                } else {
                    graph->lastVertice->nextHeadVertice = initHeadVertice(name);
                    graph->lastVertice->nextHeadVertice->prevHeadVertice = curHeadVertice;

                    graph->lastVertice = graph->lastVertice->nextHeadVertice;
                    graph->verticesNum++;
                    printf("Inserted |%s|\n\n", name);
                    return graph->lastVertice;
                }

                curHeadVertice = curHeadVertice->nextHeadVertice;
            }
        }
    }
}

void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight) {
    HeadVertice* foundHeadVertices[2];
    HeadVertice* headVerticeTarget;
    if (strcmp(name1, name2) == 0) {
        headVerticeTarget = findHeadVerticeInGraph(graph, name1);
        if (headVerticeTarget == NULL)
            headVerticeTarget = insertVerticeToGraph(graph, name1);
    } else {
        find2HeadVerticesInGraph(graph, name1, name2, foundHeadVertices);
        headVerticeTarget = foundHeadVertices[0];
        if (foundHeadVertices[0] == NULL)
            headVerticeTarget = insertVerticeToGraph(graph, name1);

        if (foundHeadVertices[1] == NULL)
            insertVerticeToGraph(graph, name2);
    }

    addConnVerticeToHeadVertice(headVerticeTarget, name2, weight);
    graph->edgesNum++;
    printf("Inserted |%s|->%u->|%s|\n\n", name1, weight, name2);
    return;
}

void deleteVerticeFromGraph(Graph* graph, char* name) {
    HeadVertice* headVerticeToDelete = findHeadVerticeInGraph(graph, name);
    if (headVerticeToDelete != NULL) {
        findAndDeleteEdgesInGraph(graph, headVerticeToDelete->name);

        if (headVerticeToDelete->firstConnVertice != NULL)
            freeConnVertice(headVerticeToDelete->firstConnVertice);

        if (headVerticeToDelete->prevHeadVertice != NULL) {
            if (headVerticeToDelete->nextHeadVertice != NULL) {
                headVerticeToDelete->prevHeadVertice->nextHeadVertice = headVerticeToDelete->nextHeadVertice;
                headVerticeToDelete->nextHeadVertice->prevHeadVertice = headVerticeToDelete->prevHeadVertice;
            } else {  // is last head vertice
                headVerticeToDelete->prevHeadVertice->nextHeadVertice = NULL;
                graph->lastVertice = headVerticeToDelete->prevHeadVertice;
            }
        } else {
            if (headVerticeToDelete->nextHeadVertice != NULL) {  // is first head vertice
                graph->firstVertice = headVerticeToDelete->nextHeadVertice;
                graph->firstVertice->prevHeadVertice = NULL;
            } else {  // is the only head vertice
                graph->firstVertice = NULL;
                graph->lastVertice = NULL;
            }
        }
        free(headVerticeToDelete->name);
        free(headVerticeToDelete);
        headVerticeToDelete = NULL;

        graph->verticesNum--;

        printf("Deleted |%s|\n\n", name);
    } else
        printf("Node |%s| does not exist - abort-d;\n\n", name);

    return;
}

char deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weight) {
    HeadVertice* headVerticeFrom = findHeadVerticeInGraph(graph, nameFrom);
    HeadVertice* headVerticeTo = findHeadVerticeInGraph(graph, nameTo);

    if (headVerticeFrom == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameFrom);
        return 1;
    }

    if (headVerticeTo == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameTo);
        return 1;
    }

    ConnVertice* connVerticeToDelete = findEdgeOfNode(graph, headVerticeFrom, nameTo, weight);  // no weight comparison if weight == -1

    if (connVerticeToDelete == NULL) {
        printf("|%s|->%u->|%s| does not exist - abort-l;\n\n", nameFrom, weight, nameTo);
        return 1;
    }

    if (connVerticeToDelete->prevConnVertice != NULL) {
        if (connVerticeToDelete->nextConnVertice != NULL) {
            connVerticeToDelete->prevConnVertice->nextConnVertice = connVerticeToDelete->nextConnVertice;
            connVerticeToDelete->nextConnVertice->prevConnVertice = connVerticeToDelete->prevConnVertice;
        } else {  // is last conn vertice
            connVerticeToDelete->prevConnVertice->nextConnVertice = NULL;
            headVerticeFrom->lastConnVertice = connVerticeToDelete->prevConnVertice;
        }
    } else {
        if (connVerticeToDelete->nextConnVertice != NULL) {  // is first head vertice
            headVerticeFrom->firstConnVertice = connVerticeToDelete->nextConnVertice;
            headVerticeFrom->firstConnVertice->prevConnVertice = NULL;
        } else {  // is the only conn vertice
            headVerticeFrom->firstConnVertice = NULL;
            headVerticeFrom->lastConnVertice = NULL;
        }
    }

    free(connVerticeToDelete->name);
    free(connVerticeToDelete);
    connVerticeToDelete = NULL;

    headVerticeFrom->connVerticesNum--;
    printf("Del-vertex |%s|->%u->|%s|\n\n", nameFrom, weight, nameTo);
    return 0;
}

char modifyEdgeOfGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weightOld, unsigned int weightNew) {
    HeadVertice* headVerticeFrom = findHeadVerticeInGraph(graph, nameFrom);
    HeadVertice* headVerticeTo = findHeadVerticeInGraph(graph, nameTo);

    if (headVerticeFrom == NULL) {
        printf("|%s| does not exist - abort-m;\n\n", nameFrom);
        return 1;
    }

    if (headVerticeTo == NULL) {
        printf("|%s| does not exist - abort-l;\n\n", nameTo);
        return 1;
    }

    ConnVertice* connVerticeToModify = findEdgeOfNode(graph, headVerticeFrom, nameTo, weightOld);

    if (connVerticeToModify == NULL) {
        printf("|%s|->%u->|%s| does not exist - abort-l;\n\n", nameFrom, weightOld, nameTo);
        return 1;
    }

    connVerticeToModify->weight = weightNew;
    printf("Mod-vertex |%s|->%u->|%s|\n\n", nameFrom, weightNew, nameTo);

    return 0;
}