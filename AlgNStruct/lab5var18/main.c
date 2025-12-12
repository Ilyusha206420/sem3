#include <stdio.h>
#include "assets/graph.h"

int main(int argc, char **argv)
{
  FILE *fp = fopen(argv[1], "r");
  Graph *g = loadMap(fp);
  for (int i = 0; i < g->size; i++) {
    for (int n = 0; n < g->size; n++) {
      printf("%d ", g->map[i][n]);
    }
    printf("\n");
  }
  return 0;
}