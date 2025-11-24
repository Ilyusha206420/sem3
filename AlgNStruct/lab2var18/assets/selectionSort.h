#pragma once
#include "myInput.h"

void selectionSort(myArray* inp) {
    int min, minPos, buf;
    for(int i = 0; i < inp->len - 1; i++) {
        min = inp->arr[i];
        minPos = i;
        for(int j = i + 1; j < inp->len; j++) { 
            if(inp->arr[j] < min) {
                min = inp->arr[j];
                minPos = j;
            }
        }

        if (minPos != i) {
            buf = inp->arr[i];
            inp->arr[i] = inp->arr[minPos];
            inp->arr[minPos] = buf;
        }
    }
}