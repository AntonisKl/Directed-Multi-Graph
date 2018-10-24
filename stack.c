#include "stack.h"

StackNode* createStackNode(HeadVertice* headVertice, unsigned int weight) {
    StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
    stackNode->headVertice = headVertice;
    stackNode->weight = weight;

    return stackNode;
}

void freeStackNode(StackNode* stackNode) {
    free(stackNode->headVertice->name);
    stackNode->headVertice->name = NULL;
    free(stackNode->headVertice);
    stackNode->headVertice = NULL;
    if (stackNode->headVerticeFrom != NULL) {
        free(stackNode->headVerticeFrom->name);
        stackNode->headVerticeFrom->name = NULL;
        free(stackNode->headVerticeFrom);
        stackNode->headVerticeFrom = NULL;
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

void copyHeadVertice(HeadVertice* to, HeadVertice* from) {
    to->visited = from->visited;
    to->firstConnVertice = from->firstConnVertice;
    to->lastConnVertice = from->lastConnVertice;
    to->prevHeadVertice = from->prevHeadVertice;
    to->nextHeadVertice = from->nextHeadVertice;
}

void pushToStack(Stack* stack, HeadVertice* headVertice, unsigned int weight, HeadVertice* headVerticeFrom) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));

    HeadVertice* headVerticeToBePushed = initHeadVertice(headVertice->name);

    copyHeadVertice(headVerticeToBePushed, headVertice);

    HeadVertice* headVerticeFromToBePushed = NULL;
    if (headVerticeFrom != NULL) {
        headVerticeFromToBePushed = initHeadVertice(headVerticeFrom->name);
        copyHeadVertice(headVerticeFromToBePushed, headVerticeFrom);
    }

    stack->stackNodes[stack->size]->headVertice = headVerticeToBePushed;
    stack->stackNodes[stack->size]->headVerticeFrom = headVerticeFromToBePushed;
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

    (*poppedStackNode)->headVertice = initHeadVertice(stackNodeToBePopped->headVertice->name);
    copyHeadVertice((*poppedStackNode)->headVertice, stackNodeToBePopped->headVertice);

    if (stackNodeToBePopped->headVerticeFrom != NULL) {
        (*poppedStackNode)->headVerticeFrom = initHeadVertice(stackNodeToBePopped->headVerticeFrom->name);
        copyHeadVertice((*poppedStackNode)->headVerticeFrom, stackNodeToBePopped->headVerticeFrom);
    } else 
        (*poppedStackNode)->headVerticeFrom = NULL;


    (*poppedStackNode)->weight = stackNodeToBePopped->weight;

    freeStackNode(stackNodeToBePopped);
    stackNodeToBePopped = NULL;
    stack->size--;
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}