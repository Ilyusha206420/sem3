#include "myLab.h"

int isOcto(char ch, FILE* fp);

char printBefore(FILE* fp, char endSym);
char printWord(FILE *f);

void printWithLightinngOctal(FILE* inpFile) {
    int inComment = 0, octo = 0;
    long op;
    char ch, buf;
    while(ch = getc(inpFile), ch != EOF) {
        if(ch == '/') {
            printf("%c", ch);
            ch = getc(inpFile);
            printf("%c", ch);
            if (ch == '/') {
                printf("%c", printBefore(inpFile, '\n'));
            }
            else if (ch == '*') {
                inComment = 1;
                while(inComment) {
                    printf("%c", printBefore(inpFile, '*'));
                    ch = getc(inpFile);
                    if(ch != EOF) {
                        printf("%c", ch);
                    }
                    if(ch == '/') {
                        inComment = 0;
                    }
                }
            }
        }
        else if (ch == '"') {
            printf("%c", ch);
            printf("%c", printBefore(inpFile, '"'));
        }
        else if(ch == '0') {
            op = ftell(inpFile);
            octo = isOcto(ch, inpFile);
            fseek(inpFile, op, SEEK_SET);
            if(octo) {
                printf("\033[92m");
            }
            printf("%c", ch);
            printf("\033[39m%c", printWord(inpFile));
        }
        else {
            printf("%c", ch);
        }
    }
}

char printBefore(FILE* fp, char endSym) {
    char ch;
    while(ch = getc(fp), ch != endSym && ch != EOF) {
        printf("%c", ch);
        if(ch == '"' && endSym != '"') printf("%c", printBefore(fp, '"'));
    }
    if(ch == EOF) return '\0';
    return ch;
}

int myStrCmp(char* str, FILE* fp, int len) {
    char ch;
    for(int i = 1; i < len; i++) {
        ch = getc(fp);
        if(ch != str[i]) {
            return 0;
        }
    }
    return 1;
}

int isOcto(char ch, FILE* fp) {
    if(ch != '0')
        return 0;
    char c;
    int len = 0;
    while(c = getc(fp), len++, c != EOF && c != ' ' && c != ';' && c != ',' && 
            c != '(' && c != ')' && c != '{' && c != '}' && c != '\n') {
        if(c < '0' || c > '7') 
            return 0;
    }
    return 1 && (len - 1);
}

char printWord(FILE *f) {
    char ch;
    while(ch = getc(f), ch != EOF && 
        (ch >= 'a' && ch <= 'z') ||
        (ch >= 'A' && ch <= 'Z') ||
        (ch >= '0' && ch <= '9') || ch == '_' && ch != EOF)
            printf("%c", ch);
    return ch;
}