#include "utils.h"

// Попытки выделить память с ограничением числа попыток.
// Возвращает NULL, если память выделить не удалось.
void* myAllocMemory(size_t size) 
{
  int attempt = 0;
  void *res = NULL;
  while (attempt++ < MAX_ALLOC_ATTEMPTS && !res) res = malloc(size);
  return res;
}