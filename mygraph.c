#include "graph.h"
#include "utils.h"

int main(int argc, char** argv) {
    char* inputFileName = NULL, *outputFileName = NULL;

    handleFlags(argc, argv, inputFileName, outputFileName);
    printf("Flags handled\n");

    Graph* graph;
    char input[50];
    if (inputFileName != NULL)
    {
        // read and create graph from input file (function)
    } else
    {
        graph = initGraph();
        printf("Graph created\n");

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
