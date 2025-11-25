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
  unsigned long numberOfPockets;
  unsigned long numberOfElements;
} HashMap;

HashMap* HMinit(unsigned long nop);
void HMfree(HashMap *hm);

int HMadd(HashMap *hm, char *key, char *val);
int HMfind(HashMap *hm, char *key, char **buf);
void HMdelete(HashMap *hm, char *key);