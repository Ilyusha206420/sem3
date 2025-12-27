#include "stack.h"

#include <stdlib.h>
#include "utils.h"
#include "myString.h"

// Реализация простого стека файлов, используемого препроцессором

FileStack* FStackInit()
{
  FileStack *out = (FileStack*)myAllocMemory(sizeof(FileStack));
  if (!out)
    return NULL;

  out->top = NULL;
  return out;
}

void FStackFree(FileStack *fs)
{
  FSnode *node = fs->top;
  while (node) {
    if (node->f) {
      if (node->f->fp)
        fclose(node->f->fp);
      if (node->f->filename)
        free(node->f->filename);
      free(node->f);
    }
    fs->top = node->next;
    free(node);
    node = fs->top;
  }
}

int FStackPush(FileStack *fs, FILE *fp, char *filename, int protection)
{
  FSnode *newNode = (FSnode*)myAllocMemory(sizeof(FSnode));
  if (!newNode)
    return -1;

  newNode->f = (FileStruct*)myAllocMemory(sizeof(FileStruct));
  if (!newNode->f) {
    free(newNode);
    return -1;
  }

  newNode->f->filename = (char*)myAllocMemory(sizeof(char) * (myStrGetLen(filename) + 1));
  if (!newNode->f->filename) {
    free(newNode->f);
    free(newNode);
    return -1;
  }
  
  newNode->f->fp = fp;
  myStrCpy(filename, &newNode->f->filename);
  newNode->f->doubleIncludeProtection = protection;
  newNode->next = fs->top;
  fs->top = newNode;

  return 0;
}

FileStruct* FStackPop(FileStack *fs)
{
  FSnode *n = fs->top;
  if (!n)
    return NULL;

  FileStruct *out = n->f;
  if (!out) 
    return NULL;

  fs->top = n->next;
  free(n);

  return out;
}