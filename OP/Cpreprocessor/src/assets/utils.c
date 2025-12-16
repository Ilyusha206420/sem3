#include "utils.h"

// выделение памяти, повторяющее попытку в случае неудачи
void* myAllocMemory(size_t size) 
{
  int try = 0;
  void *res = NULL;
  while (try++, !res && try < MAX_ALLOC_ATTEMPTS) // выделяем память, пока не будет выделена, либо не будет превышена верхняя граница попыток
    res = malloc(size);
  return res;
}