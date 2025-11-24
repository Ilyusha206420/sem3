#include <stdio.h>
#include "myLab.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("\033[91m!!! Wrong input !!!\n\033[39m");
        return -1;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if(!inputFile) {
        printf("\033[91m!!! ERROR: no such file or directory !!!\n\033[39m");
        return -1;
    }
    printf("\n");

    prototypeLining(inputFile);

    fclose(inputFile);

    printf("\n\n");
    return 0;
}