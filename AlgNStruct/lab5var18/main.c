#include <stdio.h>
#include <stdlib.h>
#include "assets/graph.h"

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Error! Wrong argument\n");
    return -1;
  }
  FILE *fp = fopen(argv[1], "r");
  if (!fp) {
    printf("Error! Can't open file: %s\n", argv[1]);
    return -1;
  }
  Graph *g = loadMap(fp);
  int start = 0;
  int finish = 0; 
  fscanf(fp, "%d", &start);
  start -= 1;
  fscanf(fp, "%d", &finish);
  finish -= 1;
  fclose(fp);

  int sum = 0;
  int prev = start;

  printf("Dijkstra algorithm:\n");
  int* dijkstraPath = dijkstra(g, start, finish);
  if (dijkstraPath) {
    printf("Path: ");
    for (int i = 0; dijkstraPath[i] != -1; i++) {
      printf("%d ", dijkstraPath[i] + 1);
      if (dijkstraPath[i + 1] != -1)
        printf("-> ");
      sum += g->map[prev][dijkstraPath[i]];
      prev = dijkstraPath[i];
    }
    printf("\nPath cost: %d\n", sum);
    free(dijkstraPath);
  } else {
    printf("No path found\n");
  }
  
  sum = 0;
  prev = start;

  printf("\nFloyd-Warshall algorithm:\n");
  int* floydPath = floyd(g, start, finish);
  if (floydPath) {
    printf("Path: ");
    for (int i = 0; floydPath[i] != -1; i++) {
      printf("%d ", floydPath[i] + 1);
      if (floydPath[i + 1] != -1)
        printf("-> ");
      sum += g->map[prev][floydPath[i]];
      prev = floydPath[i];
    }
    printf("\nPath cost: %d\n", sum);
    free(floydPath);
  } else {
    printf("No path found\n");
  }
  
  graphFree(g);
  free(g);
  return 0;
}