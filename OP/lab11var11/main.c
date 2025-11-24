#include "myLab.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Error! \n");
        return -1;
    }

    FILE *f;
    for(int i = 1; i < argc; i++) {
        f = fopen(argv[i], "r");
        if(!f) {
            printf("Error with opening file: %s\n", argv[i]);
        } else {
            printf("\033[39m%s:\n", argv[i]);
            printWithLightinngOctal(f);
            fclose(f);
        }
    }

    return 0;
}