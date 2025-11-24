#pragma once
#include <stdio.h>

typedef struct myTable {
    int len, cap;
    char** table;
} myTable;

myTable* getTableFromFile(FILE* fp);
void printTable(myTable* tp);
void clearTable(myTable* tp);

void generateHeader(myTable* tp, FILE* fp);