#include "myLab.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Error! Wrong argument");
        return -1;
    }
    FILE* fp = fopen(argv[1], "r");
    if(!fp) {
        printf("Error! Can't open file %s", argv[1]);
        return -1;
    }
    myTable* table = getTableFromFile(fp);
    fclose(fp);
    if(!table) {
        printf("Error! Something goes wrong");
        return -1;
    }
    FILE* ofp = fopen(argv[2], "w");
    if(!ofp) {
        printf("Error! Can't create file %s", argv[2]);
        return -1;
    }
    generateHeader(table, ofp);
    printf("Header file %s generated!\n", argv[2]);
    clearTable(table);
    
    return 0;
}