#include "binTree.h"

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <math.h>

TreeNode* TNinit(int key, int data) 
{
  TreeNode *out = (TreeNode*)malloc(sizeof(TreeNode));
  out->right = NULL;
  out->left = NULL;
  out->key = key;
  out->data = data;
  return out;
}

BinTree BTinit()
{
  return (BinTree) {NULL, 0};
}

BinTree BTfileInit(FILE *fp)
{
  BinTree out = BTinit();
  int data;
  while (fscanf(fp, "%d", &data) != EOF) 
    BTadd(&out, data);
  return out;
}

void BTfree(BinTree *bt) 
{
  TreeNode *tn = bt->root;
  if (!tn)
    return;
  TreeNode *prev;
  Stack s = stackInit();
  while (tn || s.top) {
    if (s.top)
      tn = stackPop(&s);
    while (tn) {
      if (tn->right)
        stackPush(tn->right, &s);
      prev = tn;
      tn = tn->left;
      free(prev);
    }
  }
  stackFree(&s);
}

void BTadd(BinTree *bt, int data)
{
  TreeNode *tn = bt->root;
  if (!tn) {
    bt->root = TNinit(1, data);
    bt->size = 1;
    return;
  }

  while (tn) {
    if (data == tn->data)
      return;
    else if (data > tn->data) {
      if (tn->right) 
        tn = tn->right;
      else {
        tn->right = TNinit(tn->key * 2 + 1, data);
        return;
      }
    }
    else if (data < tn->data) {
      if (tn->left)
        tn = tn->left;
      else {
        tn->left = TNinit(tn->key * 2, data);
        return;
      }
    }
  }
}

void BTprint(BinTree *tree) 
{
  TreeNode *tn = tree->root;
  if (!tn)
    return;
  Stack stack = stackInit();
  while (tn || stack.top) {
    if (stack.top)
      tn = stackPop(&stack);
    while (tn) {
      if (tn->right)
        stackPush(tn->right, &stack);
      for (int i = 0; i < (int)(log2(tn->key)); i++) {
        printf("    ");
      }
      printf("[%d]\n", tn->data);
      tn = tn->left;
    }
  }
  stackFree(&stack);
}

int BTckol(BinTree *bt, int level)
{
  TreeNode *tn = bt->root;
  if (!tn)
    return 0;
  
  int res = 0;
  Stack stack = stackInit();
  while (tn || stack.top) {
    if (stack.top)
      tn  = stackPop(&stack);
    while (tn) {
      if (tn->right)
        stackPush(tn->right, &stack);
      if ((int)(log2(tn->key)) + 1 == level && (tn->right || tn->left))
        res++;
      tn = tn->left;
    }
  }

  stackFree(&stack);
  return res;
}