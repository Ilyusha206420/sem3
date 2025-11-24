#pragma once
#include <stdio.h>

typedef struct TreeNode
{
  struct TreeNode *right;
  struct TreeNode *left;
  int key;
  int data;
} TreeNode;

typedef struct
{
  TreeNode *root;
  long size;
} BinTree;

BinTree BTinit();
BinTree BTfileInit(FILE *fp);
void BTfree(BinTree *bt);

void BTadd(BinTree *bt, int data);
void BTdelete(BinTree *bt, int data);

void BTprint(BinTree *bt);

int BTckol(BinTree *bt, int level);