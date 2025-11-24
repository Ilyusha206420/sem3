#include "myLab.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int* sieveOfEratosthenes(int min, int max) {
    if (min < 2) min = 2;
    if (max < min)
        return NULL;

    int size = max - min + 1;
    int *sieve = (int*)malloc(size * sizeof(int));
    
    for (int i = 0; i < size; i++)
        sieve[i] = 1;
    int limit = (int)sqrt(max);
    
    for (int p = 2; p <= limit; p++) {
        int start = ((min + p - 1) / p) * p;
        if (start < p * p) 
            start = p * p;
        
        for (int j = start; j <= max; j += p) {
            if (j >= min)
                sieve[j - min] = 0;
        }
    }
    return sieve;
}

void doublePrimes(LinkedList *list) {
    Node* n = list->first;
    int min = (list->min < 2) ? 2 : list->min;
    int* sieve = sieveOfEratosthenes(list->min, list->max);
    if(!sieve)
        return;
    while(n) {
        if(n->dat > 1) if(sieve[n->dat - min]) {
            LLpushByPos(n, n->dat);
            n = n->next;
        }
        n = n->next;
    }
    free(sieve);
}