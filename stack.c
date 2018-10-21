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
    // unsigned int i = 0;
    // for (i = 0; i < stack->maxSize; i++) {
    //     if (stack->stackNodes[i] != NULL) {
    //         free(stack->stackNodes[i]->headVertice->name);
    //         free(stack->stackNodes[i]->headVertice);
    //         if (stack->stackNodes[i]->nameFrom != NULL)
    //             free(stack->stackNodes[i]->nameFrom);

    //         free(stack->stackNodes[i]);
    //     }
    // }

    free(stack->stackNodes);
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
    // strcpy(to->name, from->name);
}

void pushToStack(Stack* stack, HeadVertice* headVertice, unsigned int weight, char* nameFrom) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }
    printf("a\n");
    HeadVertice* headVerticeToBePushed = initHeadVertice(headVertice->name);
    printf("b\n");

    copyHeadVertice(headVerticeToBePushed, headVertice);
    printf("c, %u\n", stack->size);

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));
    printf("d\n");

    if (nameFrom != NULL) {
        stack->stackNodes[stack->size]->nameFrom = allocName(nameFrom);
        strcpy(stack->stackNodes[stack->size]->nameFrom, nameFrom);
    } else
        stack->stackNodes[stack->size]->nameFrom = NULL;
    printf("e\n");

    stack->stackNodes[stack->size]->headVertice = headVerticeToBePushed;
    stack->stackNodes[stack->size]->weight = weight;
    printf("byteSize push: %lu\n", sizeof(stack->stackNodes[stack->size]));

    stack->size++;
    printf("name1: %s\n", headVerticeToBePushed->name);
    return;
}

void popFromStack(Stack* stack, StackNode** poppedStackNode) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    printf("in pop\n");
    // (poppedStackNode) = (StackNode*)malloc(sizeof(StackNode));
    printf("in pop1\n");

    StackNode* stackNodeToBePopped = stack->stackNodes[stack->size - 1];
    printf("byteSize pop: %lu\n", sizeof(stackNodeToBePopped));
    printf("in pop2\n");

    printf("popped name: %s\n", stackNodeToBePopped->headVertice->name);

    (*poppedStackNode)->headVertice = initHeadVertice(stackNodeToBePopped->headVertice->name);
    copyHeadVertice((*poppedStackNode)->headVertice, stackNodeToBePopped->headVertice);

    printf("in pop3\n");

    if (stackNodeToBePopped->nameFrom != NULL) {
        (*poppedStackNode)->nameFrom = allocName(stackNodeToBePopped->nameFrom);
        strcpy((*poppedStackNode)->nameFrom, stackNodeToBePopped->nameFrom);
    }
    printf("aaaaaaaaaaaaaaa: %s\n", (*poppedStackNode)->headVertice->name);

    printf("in pop4\n");

    (*poppedStackNode)->weight = stackNodeToBePopped->weight;
    printf("in pop5\n");

    // free(stackNodeToBePopped->headVertice->name);
    //     printf("in pop6\n");
    stackNodeToBePopped->headVertice->name = NULL;
    free(stackNodeToBePopped->headVertice);
        printf("in pop7\n");
    stackNodeToBePopped->headVertice = NULL;
    if (stackNodeToBePopped->nameFrom != NULL) {
        free(stackNodeToBePopped->nameFrom);
        stackNodeToBePopped->nameFrom = NULL;
    }
        printf("in pop8\n");

    free(stackNodeToBePopped);
        printf("in pop9\n");

    stackNodeToBePopped = NULL;

    printf("in pop10\n");
    stack->size--;
    // return stackNodeToBePopped;
    // return stack->stackNodes[stack->size];
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}