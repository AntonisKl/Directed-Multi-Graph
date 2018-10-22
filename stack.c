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
    if (stackNode->nameFrom != NULL) {
        free(stackNode->nameFrom);
        stackNode->nameFrom = NULL;
    }
    free(stackNode);
    // stackNode = NULL;
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
    //////////////////////////////////////////////////////////////////////////// may need to set them to NULL after free
    free(stack->stackNodes);
    stack->stackNodes = NULL;
    free(stack);
    stack = NULL;
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
    printf("in pop2.5\n");
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

    freeStackNode(stackNodeToBePopped);
    stackNodeToBePopped = NULL;

    printf("in pop10\n");
    stack->size--;
    // return stackNodeToBePopped;
    // return stack->stackNodes[stack->size];
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}