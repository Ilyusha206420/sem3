#include "utils.h"

void* myAllocMemory(size_t size) 
{
  int try = 0;
  void *res = NULL;
  while (try++, !res && try < MAX_ALLOC_ATTEMPTS)
    res = malloc(size);
  return res;
}