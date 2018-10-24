#include "utils.h"

// Function to remove all spaces from a given string
void removeSpaces(char* str) {
    // To keep track of non-space character count
    if (str == NULL)
        return;

    int count = 0;
    int stringSize = strlen(str);

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; i < stringSize; i++) {
        if (str[i] != ' ')
            str[count++] = str[i];  // here count is
                                    // incremented
    }
    str[count] = '\0';
    return;
}

void handleFlags(int argc, char** argv, char** inputFileName, char** outputFileName) {
    if (argc > 1) {
        if (argc == 5) {
            if (strcmp(argv[1], "-i") == 0) {
                (*inputFileName) = argv[2];
                (*outputFileName) = argv[4];
            } else if (strcmp(argv[3], "-o") == 0) {
                (*inputFileName) = argv[4];
                (*outputFileName) = argv[2];
            } else {
                printf("Invalid flags\nExiting...\n");
                exit(1);
            }
        } else if (argc == 3) {
            if (strcmp(argv[1], "-i") == 0)
                (*inputFileName) = argv[2];
            else if (strcmp(argv[1], "-o") == 0)
                (*outputFileName) = argv[2];
            else {
                printf("Invalid flags\nExiting...\n");
                exit(1);
            }
        } else {
            printf("Invalid flags\nExiting...\n");
            exit(1);
        }
    }
}

char handleInput(char* input, Graph* graph) {
    char* commandS = strtok(input, " ");
    if (commandS != NULL)
        removeSpaces(commandS);
    if (commandS == NULL || strlen(commandS) == 0)
        printf("Wrong input, try again\n");
    char command = commandS[0];
    char* arguments = strtok(NULL, "\n");

    if (arguments == NULL || strlen(arguments) == 0) {
        if (command != 'e')
            printf("Wrong input, try again\n");
        else {
            printf("Exiting...\n");
            return 1;
        }
    } else {
        char *arg1, *arg2, *arg3, *arg4;
        switch (command) {
            case 'i':
                arg1 = strtok(arguments, "\n");
                removeSpaces(arg1);
                if (arg1 == NULL || strlen(arg1) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                insertVerticeToGraph(graph, arg1);
                break;
            case 'n':
                arg1 = strtok(arguments, " ");
                removeSpaces(arg1);
                arg2 = strtok(NULL, " ");
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
                break;
            case 'd':
                arg1 = strtok(arguments, "\n");
                removeSpaces(arg1);
                if (arg1 == NULL || strlen(arg1) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                deleteVerticeFromGraph(graph, arg1);
                break;
            case 'l':
                arg1 = strtok(arguments, " ");
                removeSpaces(arg1);
                arg2 = strtok(NULL, " ");
                removeSpaces(arg2);
                arg3 = strtok(NULL, "\n");
                removeSpaces(arg3);
                if (arg1 == NULL || arg2 == NULL || strlen(arg1) == 0 || strlen(arg2) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }

                if (arg3 == NULL || strlen(arg3) == 0) {
                    while (deleteEdgeFromGraph(graph, arg1, arg2, -1) == 0)
                        ;
                } else {
                    int weight = atoi(arg3);
                    if (weight <= 0) {  // error in conversion
                        printf("Invalid weight input, try again\n");
                        break;
                    }
                    deleteEdgeFromGraph(graph, arg1, arg2, weight);
                }
                break;
            case 'm':
                arg1 = strtok(arguments, " ");
                removeSpaces(arg1);
                arg2 = strtok(NULL, " ");
                removeSpaces(arg2);
                arg3 = strtok(NULL, " ");
                removeSpaces(arg3);
                arg4 = strtok(NULL, "\n");
                removeSpaces(arg4);
                if (arg1 == NULL || arg2 == NULL || arg3 == NULL || arg4 == NULL || strlen(arg1) == 0 || strlen(arg2) == 0 || strlen(arg3) == 0 || strlen(arg4) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                int weightOld = atoi(arg3);
                if (weightOld <= 0) {  // error in conversion
                    printf("Invalid weight input, try again\n");
                    break;
                }
                int weightNew = atoi(arg4);
                if (weightNew <= 0) {  // error in conversion
                    printf("Invalid weight input, try again\n");
                    break;
                }

                modifyEdgeOfGraph(graph, arg1, arg2, weightOld, weightNew);
                break;
            case 'r':
                arg1 = strtok(arguments, "\n");
                removeSpaces(arg1);
                if (arg1 == NULL || strlen(arg1) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                printReceivingEdges(graph, arg1);
                break;
            case 'c':
                arg1 = strtok(arguments, "\n");
                removeSpaces(arg1);
                if (arg1 == NULL || strlen(arg1) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                printSimpleCirclesOfVertice(graph, arg1);
                break;
            case 'f':
                arg1 = strtok(arguments, " ");
                removeSpaces(arg1);
                arg2 = strtok(NULL, "\n");
                removeSpaces(arg2);
                if (arg1 == NULL || arg2 == NULL || strlen(arg1) == 0 || strlen(arg2) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                int minWeight = atoi(arg2);
                if (minWeight <= 0) {  // error in conversion
                    printf("Invalid weight input, try again\n");
                    break;
                }
                printAllCirclesOfVertice(graph, arg1, minWeight);
                break;
            case 't':
                arg1 = strtok(arguments, " ");
                removeSpaces(arg1);
                arg2 = strtok(NULL, " ");
                removeSpaces(arg2);
                arg3 = strtok(NULL, "\n");
                removeSpaces(arg3);
                if (arg1 == NULL || arg2 == NULL || strlen(arg1) == 0 || strlen(arg2) == 0) {
                    printf("Wrong input, try again\n");
                    break;
                }
                int l = atoi(arg3);
                if (l <= 0) {  // error in conversion
                    printf("Invalid max length input, try again\n");
                    break;
                }
                traceFlow(graph, arg1, arg2, (unsigned int) l);
                break;

            case 'p':
                printGraph(graph, "stdout");
                break;
            // case 'e':
            //     exit(0);
            default:
                printf("Wrong input, try again\n");
        }
        return 0;
    }
}
