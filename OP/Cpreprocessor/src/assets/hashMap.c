#include "hashMap.h"

#include <stdio.h>
#include <stdlib.h>
#include "myString.h"

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

  out->numberOfPockets = nop;
  out->numberOfElements = 0;

  return out;
}

void HMfree(HashMap *hm)
{
  for (int i = 0; i < hm->numberOfPockets; i++) {
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
  unsigned long newSize = hm->numberOfPockets * 2;
  HMpocket *newMap = (HMpocket*)malloc(sizeof(HMpocket) * newSize);
  int try = 0;
  while (!newMap && try < 10000) 
    newMap = (HMpocket*)malloc(sizeof(HMpocket) * newSize);
  if (!newMap)
    return;
  
  for (int i = 0; i < hm->numberOfPockets; i++) {
    HMPnode *node = hm->map[i].begin;
    while (node) {
      HMpocket *pocket = &newMap[FNV1Hash(node->key) % newSize];
      if (pocket->end) {
        pocket->end->next = node;
        pocket->end = node;
      }
      else {
        pocket->begin = node;
        pocket->end = node;
      }
      node = node->next;
    }
  }

  free(hm->map);
  hm->map = newMap;
  hm->numberOfPockets = newSize;
}

HMPnode* HMPNinit(char *key, char *val)
{
  HMPnode *out = (HMPnode*)malloc(sizeof(HMPnode));
  int try = 0;
  while (try++, !out && try < 10000) 
    out = (HMPnode*)malloc(sizeof(HMPnode));
  if (!out) 
    return NULL;
  out->key = key;
  out->val = val;
  out->next = NULL;
  return out;
}

HMPnode* HMPfind(HMpocket *pocket, char *key)
{
  HMPnode *res = pocket->begin;
  while (res) {
    if (myStrCmp(res->key, key)) {
      return res;
    }
    res = res->next;
  }
  return NULL;
}

int HMadd(HashMap *hm, char *key, char *val)
{
  if (hm->numberOfElements >= (hm->numberOfPockets / 2)) 
    HMresize(hm);

  HMpocket *pocket = &hm->map[FNV1Hash(key) % hm->numberOfPockets];
  HMPnode *elem = HMPfind(pocket, key);
  if (elem) 
    elem->val = val;
  else {
    HMPnode *newElem = HMPNinit(key, val);
    if (!newElem)
      return -1;

    if (pocket->end)
      pocket->end->next = newElem;
    else
      pocket->begin = newElem;
      
    pocket->end = newElem;

    hm->numberOfElements += 1;
  }
  
  return 0;
}

int HMfind(HashMap *hm, char *key, char **buf)
{
  HMPnode *node = HMPfind(&hm->map[FNV1Hash(key) % hm->numberOfPockets], key);
  if (!node)
    return 0;
  myStrCpy(node->val, buf);
  return 1;
}

void HMdelete(HashMap *hm, char *key)
{
  HMpocket *pocket = &hm->map[FNV1Hash(key) % hm->numberOfPockets];
  HMPnode *node = pocket->begin;
  HMPnode *prev = NULL;
  while (node && !myStrCmp(node->key, key)) {
    prev = node;
    node = node->next;
  }
  if (!node)
    return;

  if (&(pocket->begin) == &node) 
    pocket->begin = node->next;
  if (&(pocket->end) == &node)
    pocket->end = prev;
  if (prev)
    prev->next = node->next;

  free(node->key);
  free(node->val);
  free(node);
}