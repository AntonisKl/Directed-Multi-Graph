#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Comments: 
// - the Graph has a double linked list for its Vertices and a double linked list for the Edges of each of the Vertices
// - Vertices are sorted for fast search
// - Edges are sorted for fast search
// - the Graph has a head and last pointer for its Vertices
// - each Vertice has a head and last pointer for its Edges

typedef struct Edge {
    struct Edge* nextEdge;
    struct Edge* prevEdge;
    char* name;
    unsigned int weight; // unsigned int because it represents money
    char visited;
} Edge;

typedef struct Vertice {
    Edge* firstEdge;
    Edge* lastEdge;
    struct Vertice* nextVertice;
    struct Vertice* prevVertice;
    char* name;
    unsigned int edgesNum;
    char visited;
} Vertice;

typedef struct Graph {
    Vertice* firstVertice;
    Vertice* lastVertice;
    unsigned int verticesNum;
    unsigned int edgesNum;
} Graph;

char* allocName(char* name);
Vertice* initVertice(char* name);
Edge* initEdge(char* name);

Graph* initGraph();

void freeEdge(Edge* edge);
void freeVertice(Vertice* vertice);
void destroyGraph(Graph* graph);

void resetVisitedReceivingEdges(Graph* graph, char* nameTo);

void printGraph(Graph* graph, char* out);
void printReceivingEdges(Graph* graph, char* nameTo);
void printSimpleCirclesOfVertice(Graph* graph, char* name);
void printAllCirclesOfVertice(Graph* graph, char* name, unsigned int minWeight);

Vertice* findVerticeInGraph(Graph* graph, char* name);
void find2VerticesInGraph(Graph* graph, char* name1, char* name2, Vertice* foundVertices[2]);

Edge* findEdgeOfVertice(Graph* graph, Vertice* verticeFrom, char* nameTo, unsigned int weight);
void findAndDeleteEdgesInGraph(Graph* graph, char* nameTo);

void addEdgeToVertice(Vertice* vertice, char* name, unsigned int weight);
Vertice* insertVerticeToGraph(Graph* graph, char* name);
void insertEdgeToGraph(Graph* graph, char* name1, char* name2, unsigned int weight);

void deleteVerticeFromGraph(Graph* graph, char* name);

char deleteEdgeFromGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weight);

char modifyEdgeOfGraph(Graph* graph, char* nameFrom, char* nameTo, unsigned int weightOld, unsigned int weightNew);

void traceFlow(Graph* graph, char* nameFrom, char* nameTo, unsigned int pathLength);

#endif
