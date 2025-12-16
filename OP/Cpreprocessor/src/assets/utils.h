#pragma once

#include <stdlib.h>
#define MAX_ALLOC_ATTEMPTS 10000

// выделение памяти, повторяющее попытку в случае неудачи
void* myAllocMemory(size_t size);