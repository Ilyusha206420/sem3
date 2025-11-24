#include "linkedList.h"
#include <stdlib.h>

LinkedList* LLinit() 
{
  LinkedList* out = (LinkedList*)malloc(sizeof(LinkedList));
  out->first = NULL;
  out->end = NULL;
  return out;
}

void LLfree(LinkedList* l) 
{
  ListNode* n = l->first;
  ListNode* prev;
  while(n) {
    prev = n;
    n = n->next;
    free(prev);
  }
}

ListNode* LNinit(char* data)
{
  ListNode* out = (ListNode*)malloc(sizeof(ListNode));
  out->chptr = data;
  out->next = NULL;
  return out;
}

void LLpushBack(LinkedList *list, char *data)
{
  if(list->end) {
    list->end->next = LNinit(data);
    list->end = list->end->next;
  }
  else {
    list->first = LNinit(data);
    list->end = list->first;
  }
}