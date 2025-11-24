#include "assets/linkedList.h"
#include "assets/myLab.h"
#include <stdio.h>

int main() {
    int len, dat;
    LinkedList* list = LLinit();
    printf("Enter your list lenight: ");
    scanf("%d", &len);
    printf("Enter your elements: ");
    for(int i = 0; i < len; i++) {
        scanf("%d", &dat);
        LLpushBack(list, dat);
    }
    doublePrimes(list);
    printf("Your list with doublrd prime numbers: ");
    LLprint(list);
    LLfree(list);
    return 0;
}