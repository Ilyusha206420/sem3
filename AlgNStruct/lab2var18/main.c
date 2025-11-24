#include "./assets/myInput.h"
#include "assets/radixSort.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    myArray* inp = NULL;
    int inpType, len, max;
    char* fileName;
    printf("Enter input type:\n1 - random generating array\n2 - reading array from text file\n3 - reading array from keyboar\nEnter your choice: ");
    scanf("%d", &inpType);
    printf("Enter array lenight: ");
    scanf("%d", &len);
    while (getchar() != '\n');
    switch (inpType) {
        case 1: 
            printf("Enter maximum element: ");
            scanf("%d", &max);
            inp = initRandomArray(max, len);
            break;
        case 2:
            printf("Enter file name: ");
            fileName = readStrFromKB();
            inp = initFileArray(fileName, len);
            free(fileName);
            break;
        case 3:
        printf("Enter your array: ");
            inp = initShellArr(len);
            break;
        default: break;
    }
    if(!inp) {
        printf("\nError! Something goes wrong\n");
        return -1;
    }
    printf("Your array is: ");
    printArray(inp);
    printf("\nEnter sorting type:\n1 - radix sort\n2 - selection sort\nEnter yout choice: ");
    scanf("%d", &inpType);
    switch (inpType) {
        case 1:
            radixSort(inp);
            break;
        case 2:
            radixSort(inp);
            break;
        default:
            break;
    }
    printf("\nSorted array: \n");
    printArray(inp);
    freeArray(inp);
    return 0;
}