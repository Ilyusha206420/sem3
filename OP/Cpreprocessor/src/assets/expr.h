#pragma once

#include "hashMap.h"

// Простая машина для вычисления выражений в директивах `#if`.
// Поддерживает арифметику и логические операторы, а также проверку defined(name).
int evalIfExpr(char *s, HashMap *hm);