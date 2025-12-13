#pragma once

#include <stdio.h>

typedef struct
{
  size_t size;
  int **map;
} Graph;

Graph* loadMap(FILE *fp);
void graphFree(Graph *g);

int* dijkstra(Graph* g, int start, int finish);
int* floyd(Graph* g, int start, int finish);
void freeFloydResult(int** dist, size_t size);