#include "preprocessor.h"

#include <stdio.h>

int main(int argc, char **argv)
{
  char *in = "preprocessor.c";
  char *out = "processed.c";
  if (argc >= 2) 
    in = argv[1];
  if (argc >= 3) 
    out = argv[2];
  printf("%d\n", processFile((char*)in, (char*)out));

  return 0;
}