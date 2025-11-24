#include "linkedList.h"
#include <stdlib.h>
#include <stdio.h>

Node* LNinit(int newData) {
    Node* out = (Node*)malloc(sizeof(Node));
    out->dat = newData;
    out->next = NULL;
    return out;
}

LinkedList* LLinit() {
    LinkedList* out = (LinkedList*)malloc(sizeof(LinkedList));
    out->last = NULL;
    out->first = NULL;
    return out;
}

void LLpushBack(LinkedList *list, int newData) {
    if(!list->first) {
        list->first = LNinit(newData);
        list->last = list->first;
        list->min = newData, list->max = newData;
    }
    else {
        list->last->next = LNinit(newData);
        list->last = list->last->next;
        if(newData > list->max)
            list->max = newData;
        else if(newData < list->min)
            list->min = newData;
    }
}

void LLprint(LinkedList *list) {
    Node* n = list->first;
    while (n) {
        printf("%d ", n->dat);
        n = n->next;
    }
    printf("\n");
}

void LLpushByPos(Node *pos, int newData) {
    Node* n = pos->next;
    pos->next = LNinit(newData);
    pos->next->next = n;
}

void LLfree(LinkedList *list) {
    Node* prev, *cur = list->first;
    while(cur) {
        prev = cur;
        cur = cur->next;
        free(prev);
    }
    free(list);
}