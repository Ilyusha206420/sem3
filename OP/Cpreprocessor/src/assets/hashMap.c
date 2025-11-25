#include "hashMap.h"

#include <stdlib.h>

HashMap* HMinit(unsigned long nop) 
{
  HashMap *out = (HashMap*)malloc(sizeof(HashMap));
  int try = 0;
  while (try++, !out && try < 10000)
    out = (HashMap*)malloc(sizeof(HashMap));
  if (!out)
    return NULL;

  out->map = (HMpocket*)malloc(sizeof(HMpocket) * nop);
  try = 0;
  while (try++, !out->map && try < 10000)
    out->map = (HMpocket*)malloc(sizeof(HMpocket) * nop);
  if (!out->map) {
    free(out);
    return NULL;
  }

  for (int i = 0; i < nop; i++) {
    out->map[i].begin = NULL;
    out->map[i].end = NULL;
  }

  out->nummberOfPockets = nop;
  out->numberOfElements = 0;

  return out;
}

void HMfree(HashMap *hm)
{
  for (int i = 0; i < hm->nummberOfPockets; i++) {
    HMPnode *node = hm->map[i].begin;
    while (node) {
      hm->map[i].begin = node->next;
      free(node->key);
      free(node->val);
      free(node);
      node = hm->map[i].begin;
    }
  }
  free(hm->map);
  free(hm);
}

unsigned long FNV1Hash (char *str)
{
  unsigned long hval = 0xcbf29ce484222325;
  while (*str) {
    hval ^= (unsigned long)*str++;
    hval *= 0x100000001b3;
  }
  return hval;
}

void HMresize(HashMap *hm)
{
  unsigned long newSize = hm->nummberOfPockets * 2;
  HMpocket *newMap = (HMpocket*)malloc(sizeof(HMpocket) * newSize);
  int try = 0;
  while (!newMap && try < 10000) 
    newMap = (HMpocket*)malloc(sizeof(HMpocket) * newSize);
  if (!newMap)
    return;

  
}

void HMadd(HashMap *hm, char *key, char *val)
{
  if (hm->numberOfElements >= (hm->nummberOfPockets / 2)) 
    HMresize(hm);
}