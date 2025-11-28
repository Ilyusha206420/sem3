#include "preprocessor.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  printf("%d", processFile("src/preprocessor.c", "processed.c"));
  return 0;
}