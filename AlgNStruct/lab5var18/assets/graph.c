#include "graph.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

void graphFree(Graph *g)
{
  for (int i = 0; i < g->size; i++)
    free(g->map[i]);
  free(g->map);
  g->size = 0;
}

int findMinDistance(int* dist, int* visited, size_t size) 
{
  int min = INT_MAX;
  int minIndex = -1;
    
  for (size_t v = 0; v < size; v++) {
    if (!visited[v] && dist[v] <= min) {
      min = dist[v];
      minIndex = v;
    }
  }
    
  return minIndex;
}

int* dijkstra(Graph* g, int start, int finish) 
{
  int* dist = (int*)malloc(sizeof(int) * g->size);
  int* visited = (int*)calloc(g->size, sizeof(int));
  int* path = (int*)malloc(sizeof(int) * g->size);
    
  for (size_t i = 0; i < g->size; i++) {
      dist[i] = INT_MAX;
      path[i] = -1;
  }
  dist[start] = 0;
    
  for (size_t count = 0; count < g->size - 1; count++) {
    int u = findMinDistance(dist, visited, g->size);
    if (u == -1) break;
        
    visited[u] = 1;
        
    for (size_t v = 0; v < g->size; v++) {
      if (!visited[v] && g->map[u][v] && dist[u] != INT_MAX && 
        dist[u] + g->map[u][v] < dist[v]) {
        dist[v] = dist[u] + g->map[u][v];
        path[v] = u;
      }
    }
  }
    
  free(visited);
  free(dist);
    
  if (path[finish] == -1 && start != finish) {
    free(path);
    return NULL;
  }
    
  int* result = (int*)malloc(sizeof(int) * (g->size + 1));
  int current = finish;
  int index = 0;
    
  while (current != -1) {
    result[index++] = current;
    current = path[current];
  }
    
  for (int i = 0; i < index / 2; i++) {
    int temp = result[i];
    result[i] = result[index - i - 1];
    result[index - i - 1] = temp;
  }
    
  result[index] = -1;
  free(path);
    
  return result;
}

int* floyd(Graph* g, int start, int finish) {
  int** dist = (int**)malloc(sizeof(int*) * g->size);
  int** next = (int**)malloc(sizeof(int*) * g->size);
  
  for (size_t i = 0; i < g->size; i++) {
    dist[i] = (int*)malloc(sizeof(int) * g->size);
    next[i] = (int*)malloc(sizeof(int) * g->size);
      
    for (size_t j = 0; j < g->size; j++) {
      if (i == j) {
        dist[i][j] = 0;
      } else if (g->map[i][j]) {
        dist[i][j] = g->map[i][j];
      } else {
        dist[i][j] = INT_MAX;
      }
            
      if (dist[i][j] != INT_MAX && i != j) {
        next[i][j] = j;
      } else {
        next[i][j] = -1;
      }
    }
  }
    
  for (size_t k = 0; k < g->size; k++) {
    for (size_t i = 0; i < g->size; i++) {
      for (size_t j = 0; j < g->size; j++) {
        if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
          next[i][j] = next[i][k];
        }
      }
    }
  }
    
  if (next[start][finish] == -1) {
    for (size_t i = 0; i < g->size; i++) {
      free(dist[i]);
      free(next[i]);
    }
    free(dist);
    free(next);
    return NULL;
  }
    
  int* path = (int*)malloc(sizeof(int) * (g->size + 1));
  int current = start;
  int index = 0;
    
  path[index++] = start;
  while (current != finish) {
    current = next[current][finish];
    path[index++] = current;
  }
  path[index] = -1;
    
  for (size_t i = 0; i < g->size; i++) {
    free(dist[i]);
    free(next[i]);
  }
  free(dist);
  free(next);
    
  return path;
}

void freeFloydResult(int** dist, size_t size) 
{
  for (size_t i = 0; i < size; i++) 
    free(dist[i]);
  free(dist);
}