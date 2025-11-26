#include "utils.h"

#include <stdlib.h>

void* myAllocMemory(unsigned int size) 
{
  int try = 0;
  void *res = NULL;
  while (try++, !res && try < MAX_ALLOC_ATTEMPTS)
    res = malloc(size);
  return res;
}