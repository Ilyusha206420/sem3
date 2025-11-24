#pragma once

typedef struct ListNode {
  char* chptr;
  struct ListNode* next;
} ListNode;

typedef struct LinkedList {
  ListNode* first;
  ListNode* end;
} LinkedList;

LinkedList* LLinit();
void LLfree(LinkedList* l);

void LLpushBack(LinkedList* list, char* data);