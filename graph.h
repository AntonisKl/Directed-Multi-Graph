#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConnVertice {
    struct ConnVertice* nextConnVertice;
    char* name;
    unsigned int weight;
    char visited;
} ConnVertice;

typedef struct HeadVertice {
    ConnVertice* firstConnVertice;
    ConnVertice* lastConnVertice;
    struct HeadVertice* nextHeadVertice;
    char* name;
    unsigned int connVerticesNum;
    char visited;
} HeadVertice;

typedef struct Graph {
    HeadVertice* firstVertice;
    HeadVertice* lastVertice; // just a pointer without malloc
    unsigned int verticesNum;
    unsigned int edgesNum;
} Graph;

HeadVertice* allocHeadVertice();
ConnVertice* allocConnVertice();
char* allocName(char* name);

Graph* initGraph();

void freeConnVertice(ConnVertice* connVertice);
void freeHeadVertice(HeadVertice* headVertice);
void destroyGraph(Graph* graph);

HeadVertice* findHeadVerticeInGraph(Graph* graph, char* name);
HeadVertice** find2HeadVerticesInGraph(Graph* graph, char* name1, char* name2);

void addConnVerticeToHeadVertice(HeadVertice* headVertice, char* name, unsigned int weight);
void insertVerticeToGraph(Graph* graph, char* name);
void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight);

#endif
