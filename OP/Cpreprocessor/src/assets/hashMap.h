#pragma once

#include <stdlib.h>

typedef struct HMPnode
{
  char *key;
  char *val;
  struct HMPnode *next;
} HMPnode;

typedef struct 
{
  HMPnode *begin;
  HMPnode *end;
} HMpocket;

typedef struct 
{
  HMpocket *map;
  size_t numberOfPockets;
  size_t numberOfElements;
} HashMap;

HashMap* HMinit(size_t nop);
void HMfree(HashMap *hm);

int HMadd(HashMap *hm, char *key, char *val);
int HMget(HashMap *hm, char *key, char **buf);
void HMdelete(HashMap *hm, char *key);