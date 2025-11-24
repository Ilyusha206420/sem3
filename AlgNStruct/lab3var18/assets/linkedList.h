#pragma once

#include <stdarg.h>

typedef struct Node {
    int dat;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* first, *last;
    int max, min;
} LinkedList;

LinkedList* LLinit();
void LLpushBack(LinkedList* list, int newData);
void LLpushByPos(Node* pos, int newData);
void LLprint(LinkedList* list);
void LLfree(LinkedList* list);