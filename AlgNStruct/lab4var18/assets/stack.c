#include "stack.h"
#include "binTree.h"

#include <stdlib.h>

Stack stackInit()
{
  return (Stack) {NULL};
}

void stackFree(Stack *s)
{
  Node *n = s->top;
  while (n) {
    s->top = n->prev;
    free(n);
    n = s->top;
  }
}

void stackPush(TreeNode *tn, Stack *s)
{
  Node *newNode = (Node*)malloc(sizeof(Node));
  newNode->prev = s->top;
  newNode->tn = tn;
  s->top = newNode;
}

TreeNode* stackPop(Stack *s) {
  if (!s->top)
    return NULL;

  TreeNode *out = s->top->tn;
  Node *poped = s->top;
  s->top = s->top->prev;
  free(poped);
  return out;
}