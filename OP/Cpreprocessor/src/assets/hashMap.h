#pragma once

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
  unsigned long nummberOfPockets;
  unsigned long numberOfElements;
} HashMap;

HashMap* HMinit(unsigned long nop);
void HMfree(HashMap *hm);