#pragma once

#include <stdio.h>

typedef struct
{
  char *str;
  unsigned long len;
  unsigned long cap;
} myString;

myString* myStringInit(unsigned long cap);
void myStringFree(myString *ms);

int MSgetLine(myString *ms, FILE *f);

int myStrCmp(char *str1, char *str2);
unsigned long myStrGetLen(char *str);
void myStrCpy(char *str, char **buf);