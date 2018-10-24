#ifndef STACK_H
#define STACK_H

#include "graph.h"

typedef struct StackNode {
    Vertice* vertice;
    Vertice* verticeFrom;
    unsigned int weight;
} StackNode;

typedef struct Stack {
    unsigned int size;
    unsigned int maxSize;

    StackNode** stackNodes;
} Stack;

StackNode* createStackNode(Vertice* vertice, unsigned int weight);

void freeStackNode(StackNode* stackNode);

Stack* initStack(unsigned int maxSize);
Stack* destroyStack(Stack* stack);

char stackIsEmpty(Stack* stack);

void copyVertice(Vertice* to, Vertice* from);

void pushToStack(Stack* stack, Vertice* vertice, unsigned int weight, Vertice* verticeFrom);
void popFromStack(Stack* stack, StackNode** poppedStackNode);

StackNode* peekFromStack(Stack* stack);

#endif