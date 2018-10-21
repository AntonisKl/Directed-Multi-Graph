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
    // printf("freed connVertice 1\n");
    free(connVertice->name);
    free(connVertice);
    // printf("freed connVertice\n");
    return;
}

void freeHeadVertice(HeadVertice* headVertice) {
    if (headVertice->nextHeadVertice != NULL)
        freeHeadVertice(headVertice->nextHeadVertice);
    // printf("freed headVertice 1\n");
    if (headVertice->firstConnVertice != NULL)
        freeConnVertice(headVertice->firstConnVertice);
    free(headVertice->name);
    free(headVertice);
    // printf("freed headVertice\n");
    return;
}

void destroyGraph(Graph* graph) {
    if (graph != NULL) {
        if (graph->verticesNum > 0) {
            // if (graph->firstVertice != NULL) {
            freeHeadVertice(graph->firstVertice);
            // }
        } else {
            printf("Graph is empty\n");
            return;
        }
        // free(graph->lastVertice);
        free(graph);
    }
    printf("Graph was destroyed successfully\n");
    return;
}

void printGraph(Graph* graph) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        printf("|%s|\n", curHeadVertice->name);

        ConnVertice* curConnVertice = curHeadVertice->firstConnVertice;

        while (curConnVertice != NULL) {
            printf("\t%-4u->|%s|\n", curConnVertice->weight, curConnVertice->name);
            curConnVertice = curConnVertice->nextConnVertice;
        }

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
    printf("\n");

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
        printf("Vertice with name %s not found in graph\n", name);
        return;
    }

    if (graph->verticesNum == 0) {
        printf("graph is empty\n");
        return;
    }

    Stack* stack = initStack(graph->verticesNum);
    PathNode* path[graph->verticesNum];
    StackNode* poppedStackNodes[graph->verticesNum];
    unsigned int i = 0;
    for (i = 0; i < graph->verticesNum; i++) {
        path[i] = NULL;
        poppedStackNodes[i] = NULL;
    }

    printf("1\n");
    pushToStack(stack, startingHeadVertice, 0, NULL);
    printf("2\n");

    char simpleCircleFound = 0;
    i = 0;

    while (!stackIsEmpty(stack)) {
        poppedStackNodes[i] = (StackNode*)malloc(sizeof(StackNode));
        popFromStack(stack, &poppedStackNodes[i]);
        printf("name: %s\n", poppedStackNodes[i]->headVertice->name);
        printf("3\n");

        if (poppedStackNodes[i]->headVertice->visited == 0) {
            printf("4\n");

            findHeadVerticeInGraph(graph, poppedStackNodes[i]->headVertice->name)->visited = 1;
            printf("5\n");

            ConnVertice* curConnVertice = poppedStackNodes[i]->headVertice->firstConnVertice;
            printf("8\n");
            if (curConnVertice != NULL) {
                printf("HELLO i: %d\n", i);
                path[i] = (PathNode*)malloc(sizeof(PathNode));
                path[i]->name = allocName(poppedStackNodes[i]->headVertice->name);
                strcpy(path[i]->name, poppedStackNodes[i]->headVertice->name);
                printf("0000000000000000000name will be pushed in path11111111111111111111111111111111: %s\n", path[0]->name);

                if (poppedStackNodes[i]->nameFrom != NULL) {
                    path[i]->nameFrom = allocName(poppedStackNodes[i]->nameFrom);
                    strcpy(path[i]->nameFrom, poppedStackNodes[i]->nameFrom);
                } else
                    path[i]->nameFrom = NULL;
                printf("6\n");

                printf("7\n");

                path[i]->weight = poppedStackNodes[i]->weight;

                while (curConnVertice != NULL) {
                    pushToStack(stack, findHeadVerticeInGraph(graph, curConnVertice->name), curConnVertice->weight, poppedStackNodes[i]->headVertice->name);

                    curConnVertice = curConnVertice->nextConnVertice;
                }

                                printf("000000000000name will be pushed in path22222222222222222222222222222222: %s\n", path[0]->name);

                i++;
            }

            printf("9\n");

            // poppedStackNodes[i]->
        } else {
            if (strcmp(poppedStackNodes[i]->headVertice->name, name) == 0) {
                // visited[i] = (VisitedNode*)malloc(sizeof(VisitedNode));
                // visited[i]->name = allocName(poppedStackNodes[i]->headVertice->name);
                // strcpy(visited[i]->name, poppedStackNodes[i]->headVertice->name);
                // visited[i]->weight = poppedStackNodes[i]->weight;
                unsigned int size = i;
                PathNode reversedPath[size], *correctPath[size];

                char* curNameFrom = poppedStackNodes[i]->nameFrom;
                // reversedPath[0].name = allocName(path[size - 1]->name);
                // reversedPath[0].nameFrom = allocName(curNameFrom);
                // reversedPath[0].weight = path[size - 1]->weight;
                // strcpy(reversedPath[0].name, path[size - 1]->name);
                // strcpy(reversedPath[0].nameFrom, curNameFrom);
                printf("1\n");
                unsigned int k = 0;

                printf("name0 = %s\n", path[0]->name);
                for (int j = size - 1; j >= 0; j--) {
                    printf("j = %d\n", j);
                    printf("nameLOOL: %s\n", path[j]->name);
                    if (strcmp(path[j]->name, curNameFrom) == 0) {
                        reversedPath[k].name = allocName(path[j]->name);
                        strcpy(reversedPath[k].name, path[j]->name);
                        if (j != 0) {
                            reversedPath[k].nameFrom = allocName(path[j]->nameFrom);
                            strcpy(reversedPath[k].nameFrom, path[j]->nameFrom);
                        }
                        reversedPath[k].weight = path[j]->weight;
                        curNameFrom = path[j]->nameFrom;
                        k++;
                    }
                }

                unsigned int actualSize = k;
                for (int k = 0; k < actualSize; k++)
                    correctPath[k] = &reversedPath[actualSize - 1 - k];

                printf("Cir-found ");
                printf("|%s|->", correctPath[0]->name);
                for (k = 1; k < actualSize; k++) {
                    printf("%u->|%s|->", correctPath[k]->weight, correctPath[k]->name);
                }
                printf("%u->|%s|\n", poppedStackNodes[i]->weight, poppedStackNodes[i]->headVertice->name);
                simpleCircleFound = 1;
                break;
            }

            // free(poppedStackNodes[i]->headVertice->name);
            // // free(poppedStackNodes[i]->headVertice);
            // if (poppedStackNodes[i]->nameFrom != NULL)
            //     free(poppedStackNodes[i]->nameFrom);
            // free(poppedStackNodes[i]);
        }

        // free(poppedStackNodes[i]->headVertice->name);
        // // free(poppedStackNodes[i]->headVertice);
        // if (poppedStackNodes[i]->nameFrom != NULL)
        //     free(poppedStackNodes[i]->nameFrom);
        // free(poppedStackNodes[i]);
    }
    printf("10\n");

    if (simpleCircleFound == 0)
        printf("No-circle-found |%s|\n", name);

    for (i = 0; i < graph->verticesNum; i++) {
        printf("i: %d\n", i);
        if (path[i] != NULL) {
            if (path[i]->nameFrom != NULL)
                free(path[i]->nameFrom);
            // free(path[i]->name);
            free(path[i]);
        } else
            break;
    }
    printf("11\n");

    for (i = 0; i < graph->verticesNum; i++) {
        printf("i: %d\n", i);

        if (poppedStackNodes[i] != NULL) {
            free(poppedStackNodes[i]->headVertice->name);
            free(poppedStackNodes[i]->headVertice);
            if (poppedStackNodes[i]->nameFrom != NULL)
                free(poppedStackNodes[i]->nameFrom);
            free(poppedStackNodes[i]);
        } else
            break;
    }
    // free(visited);
    printf("12\n");

    // reset visited values
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        curHeadVertice->visited = 0;
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }

    // destroy stack
    destroyStack(stack);
    // free(stack->stackNodes);
    // free(stack);
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

    // printf("graph first vertice name: %s", graph->firstVertice->name);

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
    // if (graph->verticesNum == 0) {
    //     printf("Cannot search for edge in en empty graph\n");
    //     return NULL;
    // }

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
        } else if (strcmp(nameTo, curConnVertice->name) > 0)
            return NULL;

        curConnVertice = curConnVertice->nextConnVertice;
    }

    return NULL;
}

void findAndDeleteEdgesInGraph(Graph* graph, char* nameTo) {
    HeadVertice* curHeadVertice = graph->firstVertice;

    while (curHeadVertice != NULL) {
        ConnVertice* foundEdge = findEdgeOfNode(graph, curHeadVertice, nameTo, -1);

        if (foundEdge == NULL)
            printf("NULL\n");
        else
            printf("not null\n");
        while (foundEdge != NULL) {
            if (foundEdge->prevConnVertice != NULL) {
                if (foundEdge->nextConnVertice != NULL)
                    foundEdge->prevConnVertice->nextConnVertice = foundEdge->nextConnVertice;
                else {  // is last conn vertice
                    foundEdge->prevConnVertice->nextConnVertice = NULL;
                    curHeadVertice->lastConnVertice = foundEdge->prevConnVertice;
                }
                printf("hey\n");
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

            printf("num: %d\n", curHeadVertice->connVerticesNum);
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
        printf("Vertice already in graph so cannot be inserted again\n");
        return NULL;
    }

    printf("size: %d\n", graph->verticesNum);

    if (graph->verticesNum == 0) {
        graph->firstVertice = initHeadVertice(name);

        graph->lastVertice = graph->firstVertice;
        graph->verticesNum++;
        printf("Vertice with name: %s was added successfully\n", name);
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
            printf("Vertice with name: %s was added successfully\n", name);
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
                        printf("Vertice with name: %s was added successfully\n", name);

                        return curHeadVertice->nextHeadVertice;
                    }
                } else {
                    graph->lastVertice->nextHeadVertice = initHeadVertice(name);
                    graph->lastVertice->nextHeadVertice->prevHeadVertice = curHeadVertice;

                    graph->lastVertice = graph->lastVertice->nextHeadVertice;
                    graph->verticesNum++;
                    printf("Vertice with name: %s was added successfully\n", name);
                    return graph->lastVertice;
                }

                curHeadVertice = curHeadVertice->nextHeadVertice;
            }
        }
    }
}

void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight) {
    if (strcmp(name1, name2) == 0) {
        printf("Cannot connect two vertices with the same name");
        return;
    }

    HeadVertice* foundHeadVertices[2];
    find2HeadVerticesInGraph(graph, name1, name2, foundHeadVertices);
    // if (foundHeadVertices[0] == NULL && foundHeadVertices[1] == NULL)
    //     printf("Cannot connect two vertices with the same name");
    // else {
    HeadVertice* headVerticeTarget = foundHeadVertices[0];
    if (foundHeadVertices[0] == NULL) {
        headVerticeTarget = insertVerticeToGraph(graph, name1);
        if (headVerticeTarget == NULL) {
            printf("Cannot insert vertice that already exists in graph\n");
            return;
        }
    }

    if (foundHeadVertices[1] == NULL) {
        if (insertVerticeToGraph(graph, name2) == NULL) {
            printf("Cannot insert vertice that already exists in graph with name: %s\n", name2);
            return;
        }
    }

    addConnVerticeToHeadVertice(headVerticeTarget, name2, weight);
    printf("Edge from %s to %s with weight %u was successfully created\n", name1, name2, weight);
    // }
    return;
}

void deleteVerticeFromGraph(Graph* graph, char* name) {
    printf("1\n");
    HeadVertice* headVerticeToDelete = findHeadVerticeInGraph(graph, name);
    printf("2\n");
    if (headVerticeToDelete != NULL) {
        printf("edges num: %d\n", headVerticeToDelete->connVerticesNum);
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
            printf("hey\n");
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
        // headVerticeToDelete->nextHeadVertice = NULL;
        free(headVerticeToDelete);
        headVerticeToDelete = NULL;

        graph->verticesNum--;
    } else
        printf("no such vertice in graph\n");

    return;
}

char deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weight) {
    HeadVertice* headVerticeFrom = findHeadVerticeInGraph(graph, nameFrom);

    if (headVerticeFrom == NULL) {
        printf("Vertice with name \"%s\" not found in graph\n", nameFrom);
        return 1;
    }

    ConnVertice* connVerticeToDelete = findEdgeOfNode(graph, headVerticeFrom, nameTo, weight);  // no weight comparison if weight == -1

    if (connVerticeToDelete == NULL) {
        printf("Edge not found in graph");
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
        printf("hey\n");
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
    // connVerticeToDelete->nextConnVertice = NULL;
    free(connVerticeToDelete);
    connVerticeToDelete = NULL;

    headVerticeFrom->connVerticesNum--;
    return 0;
}

char modifyEdgeOfGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weightOld, unsigned int weightNew) {
    HeadVertice* headVerticeFrom = findHeadVerticeInGraph(graph, nameFrom);

    if (headVerticeFrom == NULL) {
        printf("Vertice with name \"%s\" not found in graph\n", nameFrom);
        return 1;
    }

    ConnVertice* connVerticeToModify = findEdgeOfNode(graph, headVerticeFrom, nameTo, weightOld);

    if (connVerticeToModify == NULL) {
        printf("Edge not found in graph");
        return 1;
    }

    connVerticeToModify->weight = weightNew;

    return 0;
}