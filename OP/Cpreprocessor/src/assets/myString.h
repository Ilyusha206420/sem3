#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  char *str;
  size_t len;
  size_t cap;
} myString;

myString* myStringInit(size_t cap);
void myStringFree(myString *ms);

int MSgetLine(myString *ms, FILE *f);

int myStrCmp(char *str1, char *str2);
unsigned long myStrGetLen(char *str);
void myStrCpy(char *str, char **buf);
int myStrStr(char *str1, char *str2);