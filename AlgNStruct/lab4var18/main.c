#include "assets/binTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (!argv[1]) {
    printf("Error! You must add filename\n");
    return -1;
  }
  FILE *f = fopen(argv[1], "r");
  if (!f) {
    printf("Error, can't open file %s\n", argv[1]);
    return -1;
  }
  BinTree bt = BTfileInit(f);
  if (!argv[2]) {
    printf("Error! You must add layer\n");
    return -1;
  }
  BTprint(&bt);
  printf("\nKnots on %d layer : %d\n", atoi(argv[2]), BTckol(&bt, atoi(argv[2])));
  BTfree(&bt);
  return 0;
}