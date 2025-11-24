#include <stdarg.h>
#include "assets/linkedList.h"
#include "myLab.h"
#include <stdio.h>

int wrdCmp(char* str, char* wrd);
char* LLfindWrd(LinkedList* list, char* wrd);
void markAsDel(char* chptr);
void deleteMarked(char** str);

void deleteWords(char* str, int n, ...) 
{
  LinkedList tab[128] = {[0 ... 127] = *LLinit()};
  char* c = str;
  
  if(*c && !(*c == ' ' || *c == '.' || *c == ',' || *c == ';')) {
    LLpushBack(&tab[*c], c);
  }

  while(*c) {
    if(*c == ' ' || *c == '.' || *c == ',' || *c == ';') {
      while(*c && (*c == ' ' || *c == '.' || *c == ',' || *c == ';'))
        c++;
      if(*c)
        LLpushBack(&tab[*c], c);
    }
    else {
      c++;
    }
  }

  va_list words;
  va_start(words, n);
  char* chptr;
  for(int i = 0; i < n; i++) {
    char* word = va_arg(words, char*);
    if(tab[*word].first) {
      chptr = LLfindWrd(&tab[*word], word);
      if(chptr)
        markAsDel(chptr);
    }
  }

  deleteMarked(&str);

  for(int i = 0; i < 128; i++)
    LLfree(&tab[i]);
}

char* LLfindWrd(LinkedList* list, char* word) 
{
  ListNode* n = list->first;
  while(n) {
    if(wrdCmp(n->chptr, word)) {
      return n->chptr;
    }
    n = n->next;
  }
  return NULL;
}

int wrdCmp(char* str, char* wrd) 
{
  while(*wrd) {
    if(!*str || *str != *wrd)
      return 0;
    str++;
    wrd++;
  }
  if(!*wrd || *wrd == ' ' || *wrd == '.' || *wrd == ',' || *wrd == ';')
    return 1;
  else
    return 0;
}

void markAsDel(char* chptr) 
{
  while(*chptr && *chptr != ' ' && *chptr != '.' && *chptr != ',' && *chptr != ';') {
    *chptr = (char)128;
    chptr++;
  }
}

void deleteMarked(char** str) 
{
  char* cur = *str, *write = *str;
  while(*cur) {
    if(*cur != (char)128) {
      *write = *cur;
      write++;
    }
    cur++;
  }
  while(*write) {
    *write = '\0';
    write++;
  }
}