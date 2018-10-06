#include "utils.h"

// Function to remove all spaces from a given string
void removeSpaces(char *str)
{
    // To keep track of non-space character count
    int count = 0;
    int stringSize = strlen(str);
    // printf("size: %d\n", stringSize);

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; i < stringSize; i++)
    {
    // printf("%d\n", i);
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is
                                   // incremented
    }
    str[count] = '\0';
    // printf("Spaces removed\n");
}

void handleFlags(int argc, char** argv, char* inputFileName, char* outputFileName) {
    if (argc > 1) {
        if (argc == 5) {
            if (strcmp(argv[1], "-i") == 0) {
                inputFileName = argv[2];
                outputFileName = argv[4];
            } else if (strcmp(argv[3], "-o") == 0) {
                inputFileName = argv[4];
                outputFileName = argv[2];
            } else {
                printf("Invalid flags\nExiting...\n");
                exit(1);
            }
        } else if (argc == 3) {
            if (strcmp(argv[1], "-i") == 0)
                inputFileName = argv[2];
            else if (strcmp(argv[1], "-o") == 0)
                outputFileName = argv[2];
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
    printf("Handling input...\n");
    char* commandS = strtok(input, " ");
    // printf("string: %s\n", commandS);
    if (commandS != NULL)
        removeSpaces(commandS);
    if (commandS == NULL || strlen(commandS) == 0)
        printf("Wrong input, try again\n");
    char command = commandS[0];
    // printf("Got command: %c\n", command);
    char* arguments = strtok(NULL, "\n");
    // printf("%s\n", arguments);
    // if (arguments != NULL)
    //     removeSpaces(arguments);
    if (arguments == NULL || strlen(arguments) == 0)
    {
        if (command != 'e')
            printf("Wrong input, try again\n");
        else
        {
            printf("Exiting...\n");
            return 1;
        }
    }
    else {
        printf("Got arguments\n");
        char* arg1, *arg2, *arg3;
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
                unsigned int weight = atoi(arg3);
                if (weight == 0) {  // error in conversion
                    printf("Wrong input, try again\n");
                    break;
                }
                insertEdgeToGraph(graph, arg1, arg2, weight);
                break;

            // case 'e':
            //     exit(0);
            default:
                printf("Wrong input, try again\n");
        }
        return 0;
    }
}
