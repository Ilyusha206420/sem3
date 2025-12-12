#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

Graph* loadMap(FILE *fp)
{
  Graph *out = (Graph*)malloc(sizeof(Graph));
  fscanf(fp, "%lu", &out->size);
  out->map = (int**)malloc(sizeof(int*) * out->size);
  for (size_t i = 0; i < out->size; i++) {
    out->map[i] = (int*)malloc(sizeof(int) * out->size);
    for (size_t n = 0; n < out->size; n++) {
      fscanf(fp, "%d", &out->map[i][n]);
    }
  }
  return out;
}