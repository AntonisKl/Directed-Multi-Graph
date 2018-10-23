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
    // if (stackNode->nameFrom != NULL) {
    //     free(stackNode->nameFrom);
    //     stackNode->nameFrom = NULL;
    // }
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
    // strcpy(to->name, from->name);
}

void pushToStack(Stack* stack, HeadVertice* headVertice, unsigned int weight, HeadVertice* headVerticeFrom) {
    if (stack->size >= stack->maxSize) {
        printf("Stack is full\n");
        return;
    }

    stack->stackNodes[stack->size] = (StackNode*)malloc(sizeof(StackNode));

    //printf("a\n");
    HeadVertice* headVerticeToBePushed = initHeadVertice(headVertice->name);
    //printf("b\n");

    copyHeadVertice(headVerticeToBePushed, headVertice);
    //printf("c, %u\n", stack->size);

    HeadVertice* headVerticeFromToBePushed = NULL;
    if (headVerticeFrom != NULL) {
        headVerticeFromToBePushed = initHeadVertice(headVerticeFrom->name);
        copyHeadVertice(headVerticeFromToBePushed, headVerticeFrom);
    }

    //printf("d\n");

    //printf("e\n");

    stack->stackNodes[stack->size]->headVertice = headVerticeToBePushed;
    stack->stackNodes[stack->size]->headVerticeFrom = headVerticeFromToBePushed;
    stack->stackNodes[stack->size]->weight = weight;
    //printf("byteSize push: %lu\n", sizeof(stack->stackNodes[stack->size]));

    if (headVerticeFromToBePushed != NULL)
        printf("name1: %s\n", headVerticeFromToBePushed->name);

    printf("Pushed node in stack--> ");
    if (stack->stackNodes[stack->size]->headVerticeFrom != NULL)
        printf("nameFrom: %s ", stack->stackNodes[stack->size]->headVerticeFrom->name);
    printf("name: %s, weight: %u\n", stack->stackNodes[stack->size]->headVertice->name, stack->stackNodes[stack->size]->weight);

    if (stack->stackNodes[stack->size]->headVerticeFrom != NULL) {
        printf("edges (nameFrom): ");

        ConnVertice* curConnVertice = stack->stackNodes[stack->size]->headVerticeFrom->firstConnVertice;
        while (curConnVertice != NULL) {
            printf("{|%s|, %u}, ", curConnVertice->name, curConnVertice->weight);
            curConnVertice = curConnVertice->nextConnVertice;
        }
        printf("\n");
    }
    stack->size++;

    return;
}

void popFromStack(Stack* stack, StackNode** poppedStackNode) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    //printf("in pop\n");
    // (poppedStackNode) = (StackNode*)malloc(sizeof(StackNode));
    //printf("in pop1\n");

    StackNode* stackNodeToBePopped = stack->stackNodes[stack->size - 1];
    //printf("byteSize pop: %lu\n", sizeof(stackNodeToBePopped));
    //printf("in pop2\n");

    //printf("popped name: %s\n", stackNodeToBePopped->headVertice->name);

    (*poppedStackNode)->headVertice = initHeadVertice(stackNodeToBePopped->headVertice->name);
    //printf("in pop2.5\n");
    copyHeadVertice((*poppedStackNode)->headVertice, stackNodeToBePopped->headVertice);

    //printf("in pop3\n");

    if (stackNodeToBePopped->headVerticeFrom != NULL) {
        //printf("pop3.1\n");
        (*poppedStackNode)->headVerticeFrom = initHeadVertice(stackNodeToBePopped->headVerticeFrom->name);
        copyHeadVertice((*poppedStackNode)->headVerticeFrom, stackNodeToBePopped->headVerticeFrom);
    } else {
        //printf("pop3.2\n");
        (*poppedStackNode)->headVerticeFrom = NULL;
    }

    //printf("aaaaaaaaaaaaaaa: %s\n", (*poppedStackNode)->headVertice->name);

    //printf("in pop4\n");

    (*poppedStackNode)->weight = stackNodeToBePopped->weight;
    //printf("in pop5\n");

    freeStackNode(stackNodeToBePopped);
    stackNodeToBePopped = NULL;

    printf("Popped node from stack--> ");
    if ((*poppedStackNode)->headVerticeFrom != NULL)
        printf("nameFrom: %s ", (*poppedStackNode)->headVerticeFrom->name);
    printf("name: %s, weight: %u\n", (*poppedStackNode)->headVertice->name, (*poppedStackNode)->weight);

    if ((*poppedStackNode)->headVerticeFrom != NULL) {
        printf("edges (nameFrom): ");

        ConnVertice* curConnVertice = (*poppedStackNode)->headVerticeFrom->firstConnVertice;
        while (curConnVertice != NULL) {
            printf("{|%s|, %u}, ", curConnVertice->name, curConnVertice->weight);
            curConnVertice = curConnVertice->nextConnVertice;
        }
        printf("\n");
    }

    //printf("in pop10\n");
    stack->size--;
    // return stackNodeToBePopped;
    // return stack->stackNodes[stack->size];
}

StackNode* peekFromStack(Stack* stack) {
    return stack->stackNodes[stack->size - 1];
}