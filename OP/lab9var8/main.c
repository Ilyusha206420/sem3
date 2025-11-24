#include <stdio.h>
#include <stdlib.h>

#include "myLab.h"

int main(int argc, char** argv) {
    int res;
    if(argc == 3) {
        res = copyFileAndDeleteWord(argv[1], "out.txt", atoi(argv[2]));
        if(res == -1) {
            printf("Error with openning file %s", argv[1]);
        }
        if(res == -2) {
            printf("Error with creating file");    
        }
    }
    else {
        printf("Incorrect input! Please, add minimum 2 arguments");
        return -1;
    }
    return 0;
}