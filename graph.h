#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConnVertice {
    struct ConnVertice* nextConnVertice;
    struct ConnVertice* prevConnVertice;
    char* name;
    unsigned int weight; // cause it represents money
    char visited;
} ConnVertice;

typedef struct HeadVertice {
    ConnVertice* firstConnVertice;
    ConnVertice* lastConnVertice;
    struct HeadVertice* nextHeadVertice;
    struct HeadVertice* prevHeadVertice;
    char* name;
    unsigned int connVerticesNum;
    char visited;
} HeadVertice;

typedef struct Graph {
    HeadVertice* firstVertice;
    HeadVertice* lastVertice;  // just a pointer without malloc
    unsigned int verticesNum;
    unsigned int edgesNum;
} Graph;

char* allocName(char* name);
HeadVertice* initHeadVertice(char* name);
ConnVertice* initConnVertice(char* name);

Graph* initGraph();

void freeConnVertice(ConnVertice* connVertice);
void freeHeadVertice(HeadVertice* headVertice);
void destroyGraph(Graph* graph);

void printGraph(Graph* graph);
void printReceivingEdges(Graph* graph, char* nameTo);
void printSimpleCirclesOfNode(Graph* graph, char* name);

HeadVertice* findHeadVerticeInGraph(Graph* graph, char* name);
void find2HeadVerticesInGraph(Graph* graph, char* name1, char* name2, HeadVertice* foundHeadVertices[2]);

ConnVertice* findEdgeOfNode(Graph* graph, HeadVertice* headVerticeFrom, char* nameTo, unsigned int weight);
void findAndDeleteEdgesInGraph(Graph* graph, char* nameTo);

void addConnVerticeToHeadVertice(HeadVertice* headVertice, char* name, unsigned int weight);
HeadVertice* insertVerticeToGraph(Graph* graph, char* name);
void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight);

void deleteVerticeFromGraph(Graph* graph, char* name);

char deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weight);

char modifyEdgeOfGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weightOld, unsigned int weightNew);


#endif
