#include "stack.h"

StackNode* createStackNode(HeadVertice* headVertice, unsigned int weight) {
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
    stackNode->headVertice = headVertice;
    stackNode->weight = weight;

    return stackNode;
}

Stack* initStack(unsigned int maxSize) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->stackNodes = (StackNode**)malloc(maxSize * sizeof(StackNode));
    stack->size = 0;
    stack->maxSize = maxSize;
    return stack;
}

Stack* destroyStack(Stack* stack) {
    unsigned int i = 0;
    for (i = 0; i < stack->size; i++) {
        free(stack->stackNodes[i]->headVertice->name);
        free(stack->stackNodes[i]->headVertice);
        free(stack->stackNodes[i]);
    }

    free(stack->stackNodes);
}

char stackIsEmpty(Stack* stack) {
    return stack->size == 0;
}

void copyHeadVertice(HeadVertice* to, HeadVertice* from) {
    to->visited = from->visited;
    to->firstConnVertice = from->firstConnVertice;
    to->lastConnVertice = from->lastConnVertice;
    to->prevHeadVertice = from->prevHeadVertice;
    to->nextHeadVertice = from->nextHeadVertice;
    // strcpy(to->name, from->name);
}

void pushToStack(Stack* stack, HeadVertice* headVertice, unsigned int weight) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }

    HeadVertice* headVerticeToBePushed = initHeadVertice(headVertice->name);
    copyHeadVertice(headVerticeToBePushed, headVertice);

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));

    stack->stackNodes[stack->size]->headVertice = headVerticeToBePushed;
    stack->stackNodes[stack->size]->weight = weight;
    stack->size++;

    return;
}

void popFromStack(Stack* stack, StackNode** poppedStackNode) {
    if (stack->size == 0) {
        printf("Stack is empty\n");
        return;
    }
    printf("in pop\n");
    (*poppedStackNode) = (StackNode*)malloc(sizeof(StackNode));

    StackNode* stackNodeToBePopped = stack->stackNodes[stack->size - 1];

    (*poppedStackNode)->headVertice = initHeadVertice(stackNodeToBePopped->headVertice->name);

    copyHeadVertice((*poppedStackNode)->headVertice, stackNodeToBePopped->headVertice);
    (*poppedStackNode)->weight = stackNodeToBePopped->weight;

    free(stackNodeToBePopped->headVertice->name);
    free(stackNodeToBePopped->headVertice);
    free(stackNodeToBePopped);

    printf("%s\n", (*poppedStackNode)->headVertice->name);
    stack->size--;
    // return stack->stackNodes[stack->size];
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}