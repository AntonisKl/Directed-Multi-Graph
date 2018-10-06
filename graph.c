#include "graph.h"

HeadVertice* allocHeadVertice() {
    return (HeadVertice*) malloc(sizeof(HeadVertice));
}

ConnVertice* allocConnVertice() {
    return (ConnVertice*) malloc(sizeof(ConnVertice));
}

char* allocName(char* name) {
    return (char*) malloc(sizeof(name + 1));
}


Graph* initGraph() {
    Graph* graph =  (Graph*) malloc(sizeof(Graph)); // MUST BE FREED
    // graph->firstVertice = (HeadVertice*) malloc(sizeof(HeadVertice));  // MUST BE FREED
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
        if (graph->verticesNum > 0)
        {
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


HeadVertice* findHeadVerticeInGraph(Graph* graph, char* name) {
    HeadVertice* curHeadVertice = graph->firstVertice;
    char found = 0;
    while (curHeadVertice != NULL) {
        if (strcmp(curHeadVertice->name, name) == 0) {
            found = 1;
            break;
        }
        curHeadVertice = curHeadVertice->nextHeadVertice;
    }
    return curHeadVertice;
}

void find2HeadVerticesInGraph(Graph* graph, char* name1, char* name2, HeadVertice* foundHeadVertices[2]) {
    HeadVertice *curHeadVertice1 = graph->firstVertice, *curHeadVertice2 = graph->firstVertice;
    foundHeadVertices[0] = NULL;
    foundHeadVertices[1] = NULL;

    char found1 = 0, found2 = 0;
    while (curHeadVertice1 != NULL && curHeadVertice2 != NULL) {
        if (found1 == 1) {
            if (strcmp(curHeadVertice2->name, name2) == 0) {
                found2 = 1;
                foundHeadVertices[1] = curHeadVertice2;
                break;
            }
            curHeadVertice2 = curHeadVertice2->nextHeadVertice;
        } else if (found2 == 1) {
            if (strcmp(curHeadVertice1->name, name1) == 0) {
                found1 = 1;
                foundHeadVertices[0] = curHeadVertice1;
                break;
            }
            curHeadVertice1 = curHeadVertice1->nextHeadVertice;
        } else {
            if (strcmp(curHeadVertice1->name, name1) == 0) {
                found1 = 1;
                foundHeadVertices[0] = curHeadVertice1;
            }

            if (strcmp(curHeadVertice2->name, name2) == 0) {
                found2 = 1;
                foundHeadVertices[1] = curHeadVertice2;
            }

            if (found1 == 1 && found2 == 1)
            {
                foundHeadVertices[0] = NULL;
                foundHeadVertices[1] = NULL;
                return;
            }

            curHeadVertice1 = curHeadVertice1->nextHeadVertice;
            curHeadVertice2 = curHeadVertice2->nextHeadVertice;
        }
    }
    return;
}

void addConnVerticeToHeadVertice(HeadVertice* headVertice, char* name, unsigned int weight) {
    ConnVertice* connVerticeToBeCreated;
    if (headVertice->connVerticesNum == 0)
    {
        // headVertice->firstConnVertice = allocConnVertice();
        connVerticeToBeCreated = headVertice->firstConnVertice;

        headVertice->firstConnVertice = allocConnVertice();
        headVertice->firstConnVertice->name = allocName(name);
        strcpy(headVertice->firstConnVertice->name, name);
        headVertice->firstConnVertice->weight = weight;

        headVertice->lastConnVertice = headVertice->firstConnVertice;
    } else {
        connVerticeToBeCreated = headVertice->lastConnVertice->nextConnVertice;

        headVertice->lastConnVertice->nextConnVertice = allocConnVertice();
        headVertice->lastConnVertice->nextConnVertice->name = allocName(name);
        strcpy(headVertice->lastConnVertice->nextConnVertice->name, name);
        headVertice->lastConnVertice->nextConnVertice->weight = weight;

        headVertice->lastConnVertice = headVertice->lastConnVertice->nextConnVertice;
    }
    headVertice->connVerticesNum++;
}


void insertVerticeToGraph(Graph* graph, char* name) {
    if (graph->verticesNum == 0) {
        graph->firstVertice = allocHeadVertice();
        graph->firstVertice->name = allocName(name);
        strcpy(graph->firstVertice->name, name);

        // graph->verticesNum++;
         graph->lastVertice = graph->firstVertice;
    } else {
        graph->lastVertice->nextHeadVertice = allocHeadVertice();
        graph->lastVertice->nextHeadVertice->name = allocName(name);
        strcpy(graph->firstVertice->name, name);

        graph->lastVertice = graph->lastVertice->nextHeadVertice;
    }

    graph->verticesNum++;
    printf("Vertice with name: %s was added successfully\n", name);
    return;
}

void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight) {
    HeadVertice* foundHeadVertices[2];
    find2HeadVerticesInGraph(graph, name1, name2, foundHeadVertices);
    if (foundHeadVertices[0] == NULL && foundHeadVertices[1] == NULL)
        printf("Cannot connect two vertices with the same name");
    else {
        HeadVertice* headVerticeTarget = foundHeadVertices[0];
        if (foundHeadVertices[0] == NULL)
        {
            insertVerticeToGraph(graph, name1);
            headVerticeTarget = graph->lastVertice;
        }
        else if (foundHeadVertices[1] == NULL)
            insertVerticeToGraph(graph, name2);

        addConnVerticeToHeadVertice(headVerticeTarget, name2, weight);
        printf("Edge from %s to %s with weight %u was successfully created\n", name1, name2, weight);
    }
    return;
}
