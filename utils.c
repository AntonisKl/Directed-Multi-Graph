#include "utils.h"

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
                printf("Invalid flags\nExiting...");
                exit(1);
            }
        } else if (argc == 3) {
            if (strcmp(argv[1], "-i") == 0)
                inputFileName = argv[2];
            else if (strcmp(argv[1], "-o") == 0)
                outputFileName = argv[2];
            else {
                printf("Invalid flags\nExiting...");
                exit(1);
            }
        } else {
            printf("Invalid flags\nExiting...");
            exit(1);
        }
    }
}
