#include "stack.h"

StackNode* createStackNode(Vertice* vertice, unsigned int weight) {
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
    stackNode->vertice = vertice;
    stackNode->weight = weight;

    return stackNode;
}

void freeStackNode(StackNode* stackNode) {
    free(stackNode->vertice->name);
    stackNode->vertice->name = NULL;
    free(stackNode->vertice);
    stackNode->vertice = NULL;
    if (stackNode->verticeFrom != NULL) {
        free(stackNode->verticeFrom->name);
        stackNode->verticeFrom->name = NULL;
        free(stackNode->verticeFrom);
        stackNode->verticeFrom = NULL;
    }
    free(stackNode);
}

Stack* initStack(unsigned int maxSize) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->stackNodes = (StackNode**)malloc(maxSize * sizeof(StackNode));

    stack->size = 0;
    stack->maxSize = maxSize;
    return stack;
}

Stack* destroyStack(Stack* stack) {
    if (!stackIsEmpty(stack)) {
        unsigned int i = 0;
        for (i = 0; i < stack->size; i++) {
            if (stack->stackNodes[i] != NULL) {
                freeStackNode(stack->stackNodes[i]);
                stack->stackNodes[i] = NULL;
            }
        }
    }
    free(stack->stackNodes);
    stack->stackNodes = NULL;
    free(stack);
}

char stackIsEmpty(Stack* stack) {
    return stack->size == 0;
}

void copyVertice(Vertice* to, Vertice* from) {
    to->visited = from->visited;
    to->firstEdge = from->firstEdge;
    to->lastEdge = from->lastEdge;
    to->prevVertice = from->prevVertice;
    to->nextVertice = from->nextVertice;
}

void pushToStack(Stack* stack, Vertice* vertice, unsigned int weight, Vertice* verticeFrom) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));

    Vertice* verticeToBePushed = initVertice(vertice->name);

    copyVertice(verticeToBePushed, vertice);

    Vertice* verticeFromToBePushed = NULL;
    if (verticeFrom != NULL) {
        verticeFromToBePushed = initVertice(verticeFrom->name);
        copyVertice(verticeFromToBePushed, verticeFrom);
    }

    stack->stackNodes[stack->size]->vertice = verticeToBePushed;
    stack->stackNodes[stack->size]->verticeFrom = verticeFromToBePushed;
    stack->stackNodes[stack->size]->weight = weight;
    stack->size++;

    return;
}

void popFromStack(Stack* stack, StackNode** poppedStackNode) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }

    StackNode* stackNodeToBePopped = stack->stackNodes[stack->size - 1];

    (*poppedStackNode)->vertice = initVertice(stackNodeToBePopped->vertice->name);
    copyVertice((*poppedStackNode)->vertice, stackNodeToBePopped->vertice);

    if (stackNodeToBePopped->verticeFrom != NULL) {
        (*poppedStackNode)->verticeFrom = initVertice(stackNodeToBePopped->verticeFrom->name);
        copyVertice((*poppedStackNode)->verticeFrom, stackNodeToBePopped->verticeFrom);
    } else 
        (*poppedStackNode)->verticeFrom = NULL;


    (*poppedStackNode)->weight = stackNodeToBePopped->weight;

    freeStackNode(stackNodeToBePopped);
    stackNodeToBePopped = NULL;
    stack->size--;
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}