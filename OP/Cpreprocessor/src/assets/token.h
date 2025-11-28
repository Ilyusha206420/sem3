#pragma once

#define STR_CONST 1
#define NORMAL_TOKEN 2
#define SPACES 3

#include <stdio.h>

typedef struct PreprocToken
{
  char *tok;
  int type;
  struct PreprocToken *next;
} PreprocToken;

typedef struct 
{
  PreprocToken *begin;
  PreprocToken *end;
} TokenizedLine;

TokenizedLine* TLinit();
void TLfree(TokenizedLine *tl);

void TLreadLine(TokenizedLine *tl, FILE *fp);