#pragma once

#include <stdio.h>

typedef struct
{
  size_t size;
  int **map;
} Graph;

Graph* loadMap(FILE *fp);