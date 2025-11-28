#include "myString.h"

#include "utils.h"
#include <complex.h>
#include <stdlib.h>

myString* myStringInit(unsigned long cap)
{
  myString *out = (myString*)myAllocMemory(sizeof(myString));
  if (!out)
    return NULL;

  out->str = (char*)myAllocMemory(sizeof(char) * cap);
  if (!out->str) {
    free(out);
    return NULL;
  }

  for (size_t i = 0; i < cap; i++)
    out->str[i] = '\0';

  out->cap = cap;
  out->len = 0;
  return out;
}

void myStringFree(myString *ms)
{
  free(ms->str);
  free(ms);
}

int MSgetLine(myString *ms, FILE *f)
{
  char ch;
  char prev = 0;
  unsigned long len = 0;
  int inQuot = 0;
  char qout = 0;
  while (ch = getc(f), ch != EOF && ch != '\n') {
    if (ms->cap < len + 1) {
      char *newStr = NULL;
      unsigned long newCap = ms->cap * 2;
      for (; newCap > ms->cap && !newStr; newCap--)
        newStr = (char*)myAllocMemory(sizeof(char) * newCap);
      if (!newStr)
        return -1;

      for (size_t i = 0; i < ms->cap; i++)
        newStr[i] = ms->str[i];

      for (size_t i = ms->cap; i < newCap; i++)
        newStr[i] = 0;

      free(ms->str);
      ms->str = newStr;
      ms->cap = newCap;
    }
    
    if (ch == '\'' || ch == '"') {
      if (inQuot) {
        if (ch == qout) {
          qout = 0;
          inQuot = 0;
        }
      }
      else {
        qout = ch;
        inQuot = 1;
      }
    }

    if (ch == '\\') {
      prev = ch;
      ch = getc(f);
      if (ch == '\n')
        continue;
      else {
        ms->str[len++] = prev;
        ms->str[len++] = ch;
      }
    }

    else if (ch == '/' && !inQuot) {
      prev = ch;
      ch = getc(f);
      switch (ch) {
        case '/' :
          while (ch = getc(f), ch != '\n' && ch != EOF) {
            if (ch == '\\') {
              ch = getc(f);
              if (ch == '\n')
                continue;
            }
          }
          break;
        case '*' :
          while (ch = getc(f), !(ch == '/' && prev == '*') && ch != EOF) 
            prev = ch;
          break;

        default:
          ms->str[len++] = prev;
          ms->str[len++] = ch;
      }
    }

    else 
      ms->str[len++] = ch;
  }
  for (size_t i = len; i < ms->cap; i++)
    ms->str[i] = '\0';
  ms->len = len;

  if (ch == EOF)
    return 1;
  return 0;
}

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

int myStrStr(char *str1, char *str2)
{
  while (*str1 && *str2) {
    if (*str1++ != *str2++)
      return 0;
  }
  return *str2 == '\0';
}