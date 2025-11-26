#pragma once

#include <stdio.h>

typedef struct FSnode
{
  FILE *fp;
  struct FSnode *next;
} FSnode;

typedef struct
{
  FSnode *top;
} FileStack;

FileStack* FSinit();
void FSfree(FileStack *fs);

void FSpush(FileStack *fs, FILE *fp);
FILE* FSpop(FileStack *fs);