#pragma once

#include <stdio.h>

typedef struct
{
  FILE *fp;
  char *filename;
  int doubleIncludeProtection;
} FileStruct;

typedef struct FSnode
{
  FileStruct *f;
  struct FSnode *next;
} FSnode;

typedef struct
{
  FSnode *top;
} FileStack;

FileStack* FStackInit();
void FStackFree(FileStack *fs);

int FStackPush(FileStack *fs, FILE *fp, char *filename, int protection);
FileStruct* FStackPop(FileStack *fs);

// Небольшая обёртка: стек файлов для обработки директив `#include`.
// Каждая вершина хранит FILE* и имя файла, а также флаг защиты от двойного включения.