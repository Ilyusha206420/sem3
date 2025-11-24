#pragma once
#include "myInput.h"
#include <stdlib.h>

void radixSort(myArray *inp) {
    int running = 1, maxRadix = 1;
    while(running) {
        running = 0;
        for(int i = 0; i < inp->len; i++) {
            if(inp->arr[i] / maxRadix) {
                maxRadix *= 10;
                running = 1;
                break;
            }
        }
    }
    int counters[10];
    int** sortingTable = (int**)malloc(sizeof(int*) * 10);
    if(!sortingTable) {
        return;
    }
    for(int i = 0; i < 10; i++) {
        sortingTable[i] = (int*)malloc(sizeof(int) * inp->len);
        if(!sortingTable[i])
            return;
    }
    int corRadix, m;
    for(int c = 10; c <= maxRadix; c *= 10) {
        for(int i = 0; i < 10; i++)
            counters[i] = 0;
        for(int n = 0; n < inp->len; n++) {
           corRadix = (inp->arr[n] % c) / (c / 10);
            sortingTable[corRadix][counters[corRadix]++] = inp->arr[n];
        }
        m = 0;
        for(int i = 0; i < 10; i++) {
            for(int j = 0; j < counters[i]; j++)
            inp->arr[m++] = sortingTable[i][j];
        }
    }
}