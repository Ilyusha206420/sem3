#pragma once
#include "binTree.h"

typedef struct Node 
{
  TreeNode *tn;
  struct Node *prev;
} Node;

typedef struct
{
  Node *top;
} Stack;

Stack stackInit();
void stackFree(Stack *s);

void stackPush(TreeNode *tn, Stack *s);
TreeNode* stackPop(Stack *s);