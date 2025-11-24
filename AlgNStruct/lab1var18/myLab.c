#include "myLab.h"

int isProto(char ch, FILE* fp);

char printBefore(FILE* fp, char endSym);

void prototypeLining(FILE* inpFile) {
    int inComment = 0, proto = 0;
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
        else if (ch == '#') {
            printf("%c", ch);
            printf("%c", printBefore(inpFile, '\n'));
        }
        else if(ch == 'c' || ch == 'u' || ch == 's' || ch == 'l' ||
                ch == 'i' || ch == 'b' || ch == 'f' || ch == 'd') {
            op = ftell(inpFile);
            proto = isProto(ch, inpFile);
            fseek(inpFile, op, SEEK_SET);
            if(proto) {
                printf("\033[92m");
            }
            printf("%c", ch);
            printf("%c", printBefore(inpFile, ';'));
            printf("\033[39m");
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

int isProto(char ch, FILE* fp) {
    switch(ch) {
        case 'c':
            if(!myStrCmp("char ", fp, 5)) return 0;
            break;
        case 'u':
            if(!myStrCmp("unsigned ", fp, 9)) return 0;
            break;
        case 's':
            if(!myStrCmp("short ", fp, 6)) return 0;
            break;
        case 'l':
            if(!myStrCmp("long ", fp, 5)) return 0;
            break;
        case 'i':
            if(!myStrCmp("int ", fp, 4)) return 0;
            break;
        case 'b':
            if(!myStrCmp("bool ", fp, 5)) return 0;
            break;
        case 'f':
            if(!myStrCmp("float ", fp, 6)) return 0;
            break;
        case 'd':
            if(!myStrCmp("double ", fp, 7)) return 0;
            break;
    }
    char c;
    int par = 0;
    while(c = getc(fp), c != '=' && c != '{' && c != ';' && c != EOF) {
        if(c == '(') par = 1;
    }
    if(c == ';' && par) return 1; 
    return 0;
}