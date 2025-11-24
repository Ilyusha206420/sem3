#include "myInput.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct myStr{
    int len, cap;
    char* str;
} myStr;

myArray* initEmptyArray(int arrLen) {
    myArray* out = (myArray*)malloc(sizeof(myArray));
    out->len = arrLen;
    out->arr = (int*)malloc(sizeof(int) * arrLen);
    return out;
}

myArray* initRandomArray(int max, int arrLen) {
    myArray* out = initEmptyArray(arrLen);
    srand(time(NULL));
    for(int i = 0; i < arrLen; i++)
        out->arr[i] = rand() % (max + 1);
    return out;
}

myArray* initFileArray(char* fileName, int arrLen) {
    myArray* out = initEmptyArray(arrLen);
    for(int i = 0; i < arrLen; i++)
        out->arr[i] = 0;
    FILE* ifp = fopen(fileName, "r");
    if(!ifp)
        return NULL;
    char ch;
    int i = -1;
    while(ch != EOF) {
        if(ch >= '0' && ch <= '9') {
            out->arr[i] *= 10;
            out->arr[i] += ch - '0';
            ch = getc(ifp);
        }
        else {
            while(ch = getc(ifp), !(ch >= '0' && ch <= '9') && ch != EOF);
            i++;
        }
    }
    fclose(ifp);
    return out;
}


myArray* initShellArr(int arrLen) {
    myArray* out = initEmptyArray(arrLen);
    printf("Please, input your array: ");
    for(int i = 0; i < arrLen; i++) 
        scanf("%d", &out->arr[i]);
    return out;
}

void printArray(myArray* inp) {
    printf("[");
    for(int i = 0; i < inp->len - 1; i++)
        printf("%d, ", inp->arr[i]);
    printf("%d]\n", inp->arr[inp->len - 1]);
}

void freeArray(myArray *inp) {
    if(inp->arr)
        free(inp->arr);
    free(inp);
}

myStr* newStr() {
    myStr* out = (myStr*)malloc(sizeof(myStr));
    
    out->cap = 10;
    out->len = 0;
    out->str = (char*)malloc(sizeof(char) * out->cap);
    
    return out;
}

void reallocStr(myStr *s, int newSize) {
    char* newS = (char*)malloc(sizeof(char) * newSize);
    if(newSize < s->len) 
        return;
    for(int i = 0; i < s->len; i++) {
        newS[i] = s->str[i];
    }
    free(s->str);
    s->str = newS;
    s->cap = newSize;
}

void addToStr(myStr *s, char c) {
    if(s->len == (s->cap - 1))
        reallocStr(s, (s->cap + 10));
    s->str[(s->len)++] = c;
}

char* readStrFromKB() {
    myStr* inp = newStr();
    char ch;
    while(ch = getchar(), ch != '\n') 
        addToStr(inp, ch);
    addToStr(inp, '\0');
    reallocStr(inp, inp->len);
    char* out = inp->str;
    free(inp);
    return out;
}