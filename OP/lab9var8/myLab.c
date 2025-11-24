#include <stdio.h>

#include "myLab.h"

int copyFileAndDeleteWord(char* ifn, char* ofn, int N) {
    FILE* ifp = fopen(ifn, "r");
    FILE* ofp = fopen(ofn, "w");
    if(!ifp) 
        return -1;
    if(!ofp)
        return -2;
    char ch, buf;
    int n = 0;
    while(ch = getc(ifp), ch != EOF) {
        if(buf != ' ' && ch == ' ') 
            ++n;
        if(ch == '\n')
            n = 0;
        if(n != N - 1 || ch == '\n')
            putc(ch, ofp);
        buf = ch;
    }
    fclose(ifp);
    fclose(ofp);
    return 0;
}