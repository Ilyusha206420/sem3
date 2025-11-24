#pragma once

typedef struct myArray{
    int len;
    int* arr;
} myArray;

myArray* initRandomArray(int max, int arrLen);
myArray* initShellArr(int arrLen);
myArray* initFileArray(char* fileName, int arrLen);

void printArray(myArray* inp);
void freeArray(myArray *inp);

char* readStrFromKB();