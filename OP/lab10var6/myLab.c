#include "myLab.h"
#include <stdio.h>
#include <stdlib.h>

void clearTable(myTable *tp) {
    if(!tp)
        return;
    for(int i = 0; i < tp->len; i++)
        free(tp->table[i]);
    free(tp);
}

void realocTable
(
    myTable* tp, 
    int newSize
) 
{
    char** newTable = (char**)malloc(sizeof(char*) * newSize);
    if(!newTable) {
        clearTable(tp);
        return;
    }
    for(int i = 0; i < tp->len; i++)
        newTable[i] = tp->table[i];
    free(tp->table);
    tp->table = newTable;
}

void 
addToTable
(
    myTable* 
    tp, 
    char* 
    newElem
) 
    {
    if(tp->len == (tp->cap - 1))
        realocTable(tp, tp->cap + 10);
    if(!tp)
        return;
    tp->table[tp->len++] = newElem;
}

void addToStr(char** str, char ch, int* len, int* cap) {
    if(*len == (*cap - 1)) {
        char* newStr = (char*)malloc(sizeof(char) * (*cap + 10));
        for(int i = 0; i < *cap; i++) {
            newStr[i] = (*str)[i];
        }
        *cap += 10;
        free(*str);
        *str = newStr;
    }
    (*str)[(*len)++] = ch;
}

void deleteSpaces(char** str, int len) {
    char* outStr = (char*)malloc(sizeof(char) * len);
    int spaces = 0, n = 0;
    char last;
    for(int i = 0; i < len; i++) {
        if((*str)[i] == ' ')
            spaces += 1;
        else
            spaces = 0;
        if(spaces <= 1 && !((last == '(' || last == ')') && (*str)[i] == ' ')) {
            outStr[n++] = (*str)[i];
            last = (*str)[i];
        }
    }
    free(*str);
    *str = malloc(sizeof(char) * n);
    for(int i = 0; i < n; i++)
        (*str)[i] = outStr[i];
    free(outStr);
}

char* getFuncInter(FILE* fp, char c) {
    if(c == '\n' || c == '=' || c == EOF || c == ';' || c == '{' || c == '\n' || c == '}')
        return NULL;
    char* str = (char*)malloc(sizeof(char) * 30);
    char* out = NULL;
    if(!str) 
        return NULL;
    str[0] = c;
    int cap = 30, len = 1, log = 0, spaces = 0;
    char ch, *newStr;
    while(ch = getc(fp), ch != EOF && ch != ';' && ch != '{' && ch != '}' && ch != '=' && ch != '>' && ch != '"') { 
        if(ch == '(' && log == 0)
            log = 1;
        if(ch == ')' && log == 1)
            log = 2;
        if(ch == ' ' && !log)
            spaces++;
        if(ch != '\n')
            addToStr(&str, ch, &len, &cap);
    }
    if(spaces && log == 2 && ch == '{') {
        if(str[len - 1] != ' ')
            addToStr(&str, ';', &len, &cap);
        else 
            str[len - 1] = ';';
        addToStr(&str, '\0', &len, &cap);
        out = (char*)malloc(sizeof(char) * len);
        for(int i = 0; i < len; i++)
            out[i] = str[i];
        deleteSpaces(&out, len);
    }
    free(str);
    return out;
}

myTable* getTableFromFile(FILE* fp) {
    myTable* out = (myTable*)malloc(sizeof(myTable));
    if(!out)
        return NULL;
    out->table = (char**)malloc(sizeof(char*) * 10);
    if(!out->table) {
        free(out);
        return NULL;
    }
    out->cap = 10;
    out->len = 0;

    char ch, *str;
    while (ch = getc(fp), ch != EOF) {
        while(ch == ' ' || ch == '\n')
            ch = getc(fp);
        str = getFuncInter(fp, ch);
        if(str)
            addToTable(out, str);
    }
    realocTable(out, out->cap);
    return out;
}

void printTable(myTable *tp) {
    for(int i = 0; i < tp->len; i++)
        printf("%s\n", tp->table[i]);
}

void generateHeader(myTable *tp, FILE *fp) {
    for(int i = 0; i < tp->len; i++) { 
        fputs(tp->table[i], fp);
        fputc('\n', fp);
    }
}