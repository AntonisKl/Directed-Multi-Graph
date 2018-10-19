#include "graph.h"

HeadVertice* allocHeadVertice() {
    return (HeadVertice*)malloc(sizeof(HeadVertice));
}

ConnVertice* allocConnVertice() {
    return (ConnVertice*)malloc(sizeof(ConnVertice));
}

char* allocName(char* name) {
    return (char*)malloc(sizeof(name + 1));
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

void printGraphVertices(Graph* graph) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    while (curHeadVertice != NULL) {
        printf("%s->", curHeadVertice->name);
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
    printf("\n");

    return;
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

    char found1 = 0, found2 = 0, passed1= 0, passed2 = 0;
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

ConnVertice* findEdgeInGraph(Graph* graph, HeadVertice* headVerticeFrom, char* nameTo, int weight) {
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
        ConnVertice* foundEdge = findEdgeInGraph(graph, curHeadVertice, nameTo, -1);

        if (foundEdge != NULL) {
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
        }

        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
}

void addConnVerticeToHeadVertice(HeadVertice* headVertice, char* name, unsigned int weight) {
    if (headVertice->connVerticesNum == 0) {
        // headVertice->firstConnVertice = allocConnVertice();
        headVertice->firstConnVertice = allocConnVertice();
        headVertice->firstConnVertice->name = allocName(name);
        strcpy(headVertice->firstConnVertice->name, name);
        headVertice->firstConnVertice->weight = weight;

        headVertice->lastConnVertice = headVertice->firstConnVertice;
        headVertice->connVerticesNum++;
        return;
    } else {
        ConnVertice* curConnVertice = headVertice->firstConnVertice;

        if (strcmp(name, curConnVertice->name) < 0) {
            ConnVertice* connVerticeToInsert = allocConnVertice();
            connVerticeToInsert->nextConnVertice = curConnVertice;
            connVerticeToInsert->name = allocName(name);
            strcpy(connVerticeToInsert->name, name);
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
                        ConnVertice* connVerticeToInsert = allocConnVertice();
                        connVerticeToInsert->prevConnVertice = curConnVertice;
                        connVerticeToInsert->nextConnVertice = curConnVertice->nextConnVertice;
                        connVerticeToInsert->name = allocName(name);
                        strcpy(connVerticeToInsert->name, name);
                        connVerticeToInsert->weight = weight;

                        curConnVertice->nextConnVertice->prevConnVertice = connVerticeToInsert;
                        curConnVertice->nextConnVertice = connVerticeToInsert;
                        headVertice->connVerticesNum++;
                        return;
                    }
                } else {
                    headVertice->lastConnVertice->nextConnVertice = allocConnVertice();
                    headVertice->lastConnVertice->nextConnVertice->prevConnVertice = curConnVertice;
                    headVertice->lastConnVertice->nextConnVertice->name = allocName(name);
                    strcpy(headVertice->lastConnVertice->nextConnVertice->name, name);
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

    if (graph->verticesNum == 0) {
        graph->firstVertice = allocHeadVertice();
        graph->firstVertice->name = allocName(name);
        strcpy(graph->firstVertice->name, name);

        graph->lastVertice = graph->firstVertice;
        graph->verticesNum++;
        printf("Vertice with name: %s was added successfully\n", name);
        return graph->firstVertice;
    } else {
        HeadVertice* curHeadVertice = graph->firstVertice;
        // insert at start

        if (strcmp(name, curHeadVertice->name) < 0) {
            HeadVertice* headVerticeToInsert = allocHeadVertice();
            headVerticeToInsert->nextHeadVertice = curHeadVertice;
            headVerticeToInsert->name = allocName(name);
            strcpy(headVerticeToInsert->name, name);

            curHeadVertice->prevHeadVertice = headVerticeToInsert;
            graph->firstVertice = headVerticeToInsert;
            graph->verticesNum++;
            printf("Vertice with name: %s was added successfully\n", name);
            return graph->firstVertice;
        } else {
            while (curHeadVertice != NULL) {
                if (curHeadVertice->nextHeadVertice != NULL) {
                    if (strcmp(name, curHeadVertice->nextHeadVertice->name) < 0) {
                        HeadVertice* headVerticeToInsert = allocHeadVertice();
                        headVerticeToInsert->prevHeadVertice = curHeadVertice;
                        headVerticeToInsert->nextHeadVertice = curHeadVertice->nextHeadVertice;
                        headVerticeToInsert->name = allocName(name);
                        strcpy(headVerticeToInsert->name, name);

                        curHeadVertice->nextHeadVertice->prevHeadVertice = headVerticeToInsert;
                        curHeadVertice->nextHeadVertice = headVerticeToInsert;
                        graph->verticesNum++;
                        printf("Vertice with name: %s was added successfully\n", name);

                        return curHeadVertice->nextHeadVertice;
                    }
                } else {
                    graph->lastVertice->nextHeadVertice = allocHeadVertice();
                    graph->lastVertice->nextHeadVertice->name = allocName(name);
                    strcpy(graph->lastVertice->nextHeadVertice->name, name);
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
        findAndDeleteEdgesInGraph(graph, headVerticeToDelete->name);

        if (headVerticeToDelete->firstConnVertice != NULL)
            freeConnVertice(headVerticeToDelete->firstConnVertice);

        if (headVerticeToDelete->prevHeadVertice != NULL) {
            if (headVerticeToDelete->nextHeadVertice != NULL)
                headVerticeToDelete->prevHeadVertice->nextHeadVertice = headVerticeToDelete->nextHeadVertice;
            else {  // is last head vertice
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

        graph->verticesNum--;
    } else
        printf("no such vertice in graph\n");

    return;
}

void deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, int weight) {
    HeadVertice* headVerticeFrom = findHeadVerticeInGraph(graph, nameFrom);

    if (headVerticeFrom == NULL) {
        printf("Vertice with name \"%s\" not found in graph\n", nameFrom);
        return;
    }

    ConnVertice* connVerticeToDelete = findEdgeInGraph(graph, headVerticeFrom, nameTo, weight);  // no weight comparison if weight == -1

    if (connVerticeToDelete == NULL) {
        printf("Edge not found in graph");
        return;
    }

    if (connVerticeToDelete->prevConnVertice != NULL) {
        if (connVerticeToDelete->nextConnVertice != NULL)
            connVerticeToDelete->prevConnVertice->nextConnVertice = connVerticeToDelete->nextConnVertice;
        else {  // is last conn vertice
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

    headVerticeFrom->connVerticesNum--;
}
