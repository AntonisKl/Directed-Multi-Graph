#ifndef STACK_H
#define STACK_H

#include "graph.h"

typedef struct StackNode {
    HeadVertice* headVertice;
    char* nameFrom;
    unsigned int weight;
} StackNode;

typedef struct Stack {
    unsigned int size;
    unsigned int maxSize;

    StackNode** stackNodes;
} Stack;

StackNode* createStackNode(HeadVertice* headVertice, unsigned int weight);

void freeStackNode(StackNode* stackNode);

Stack* initStack(unsigned int maxSize);
Stack* destroyStack(Stack* stack);

char stackIsEmpty(Stack* stack);

void copyHeadVertice(HeadVertice* to, HeadVertice* from);

void pushToStack(Stack* stack, HeadVertice* headVertice, unsigned int weight, char* nameFrom);

void popFromStack(Stack* stack, StackNode** poppedStackNode);

StackNode* peekFromStack(Stack* stack);

#endif