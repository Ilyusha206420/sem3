#include "myLab.h"

#include <stdio.h>
#include <stdlib.h>

#define CAPACITY 100

int isVowel(char ch) {
    if(ch >= 'A' && ch <= 'Z') {
        ch += 32;
    }
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'y';
}

char* removeAllVowels(char* str, int len) {
    char* out = (char*)malloc(len);
    char* strptr = str;
    char* outptr = out;

    while(*strptr) {
        if(!isVowel(*strptr)) {
            while(*strptr && *strptr != ' ') {
                *(outptr++) = *(strptr++);
            }
            *outptr++ = ' ';
        }
        else {
            while(*strptr && *strptr != ' ') {
                strptr++;
            }
        }
        if(*strptr == ' ') {
            strptr++;
        }
    }

    *outptr = '\0';
    
    char* resizedOut = (char*)malloc(strptr - outptr + 1);
   
    strptr = out;
    outptr = resizedOut;
    
    while (*strptr) {
        *(outptr++) = *(strptr++);
    }

    free(out);

    *outptr = '\0';

    return resizedOut;
}

typedef struct ListNode {
    char* str;
    struct ListNode* next;
} ListNode;

typedef struct LinkedList {
    ListNode* first, *end;
    int len, cap;
} LinkedList;

ListNode* createListNode() {
    ListNode* newListNode = (ListNode*)malloc(sizeof(ListNode));
    newListNode->str = (char*)malloc(CAPACITY);
    newListNode-> next = NULL;
    
    return newListNode;
}

char* getInputFromKeyboard(int* len) {
    LinkedList* input = (LinkedList*)malloc(sizeof(LinkedList));
    input->cap = CAPACITY;
    input->len = 0;
    input->first = createListNode();
    input->end = input->first;

    char ch;
    char* ptr = input->first->str;

    while(ch = getchar(), ch != '\n') {
        if(input->len == input->cap - 1) {
            input->end->next = createListNode();
            input->end = input->end->next;
            *ptr = '\0';
            ptr = input->end->str;
            input->cap += CAPACITY;
        }
        *ptr = ch;
        ptr++;
        input->len++;
    }

    *ptr = '\0';

    char* out = (char*)malloc(input->len + 1);
    char* strptr = out;
    ptr = input->first->str;
    ListNode* firstElem;

    while(input->first) {
        ptr = input->first->str;
        while(*ptr) {
            *(strptr++) = *(ptr++);
        }
    
        firstElem = input->first;
        input->first = input->first->next;
    
        free(firstElem->str);
        free(firstElem);
    }

    *strptr = '\0';

    *len = input->len + 1;
    free(input);

    return out;
}

char* getInput(int argc, char** argv, int* len) {
    char* out = NULL;

    switch(argc) {
        case 1: {
            printf("Plese, enter your string: ");
            out = getInputFromKeyboard(&*len);
            break;
        }
        case 2: {
            char* ptr = argv[1];
            while(*ptr) {
                ptr++;
            }
            *len = ptr - argv[1] + 1;
            out = argv[1];
            break;
        }
        default: {
            printf("Incorrect input, please, use \"your str\" format for entering your string.");
            break;
        }
    }

    return out;
}