#include "myString.h"

int myStrCmp(char *str1, char *str2)
{
  while (*str1 && *str2) {
    if (*str1++ != *str2++)
      return 0;
  }
  return (*str1 == '\0' && *str2 == '\0');
}

unsigned long myStrGetLen(char *str)
{
  char *c = str;
  while (*c) 
    c++;
  return c - str;
}

void myStrCpy(char *str, char **buf)
{
  char *c = *buf;
  while (*str) {
    *c++ = *str++;
  }
  *c = '\0';
}