#include "graph.h"
#include "utils.h"

int main(int argc, char** argv) {
    char *inputFileName = NULL, *outputFileName = NULL;

    handleFlags(argc, argv, &inputFileName, &outputFileName);
    printf("Flags handled\n");

    Graph* graph;
    graph = initGraph();
    printf("Graph created\n");
    char input[50];
    if (inputFileName != NULL) {
        FILE* fp;
        char* line = NULL;
        size_t len = 0;
        size_t read;
        fp = fopen(inputFileName, "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);

        char *arg1, *arg2, *arg3;
        while ((read = getline(&line, &len, fp)) != -1) {

            arg1 = strtok(line, "\t");
            removeSpaces(arg1);
            arg2 = strtok(NULL, "\t");
            removeSpaces(arg2);
            arg3 = strtok(NULL, "\n");
            removeSpaces(arg3);
            if (arg1 == NULL || arg2 == NULL || arg3 == NULL || strlen(arg1) == 0 || strlen(arg2) == 0 || strlen(arg3) == 0) {
                printf("Wrong input, try again\n");
                break;
            }
            int weight = atoi(arg3);
            if (weight <= 0) {  // error in conversion
                printf("Invalid weight input, try again\n");
                break;
            }
            insertEdgeToGraph(graph, arg1, arg2, weight);

        }

        fclose(fp);
        if (line)
            free(line);

        while (1) {
            printf("Please type a command: ");
            fgets(input, 50, stdin);
            printf("You typed: %s", input);

            int result = handleInput(input, graph);
            if (result == 1)
                break;
        }

        if (outputFileName != NULL)
            printGraph(graph, outputFileName);
        else
            printGraph(graph, "stdout");
    } else {
        while (1) {
            printf("Please type a command: ");
            fgets(input, 50, stdin);
            printf("You typed: %s", input);

            int result = handleInput(input, graph);
            if (result == 1)
                break;
        }
    }

    destroyGraph(graph);

    return 0;
}
