#pragma once

#include <stdlib.h>
#define MAX_ALLOC_ATTEMPTS 10000

// Утилиты общего назначения
// Попытки выделения памяти с несколькими рекуперациями
void* myAllocMemory(size_t size);