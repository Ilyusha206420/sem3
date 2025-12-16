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
  int error = processFile(in, out);
  switch (error) {
    case 0:
    break;
    case FILE_OPEN_ERROR:
      printf("Error! Can't open file: %s\n", in);
    break;
    case FILE_CREATE_ERROR:
      printf("Error! Can't create file: %s\n", out);
    break;
    case MEMORY_ALLOC_ERROR:
      printf("Error! Too little RAM\n");
    break;
    case HEADER_FILE_OPENINNG_ERROR:
      printf("Error! Can't open included file!\n");
    break;
    case STACK_ERROR:
      printf("Error! Can't allocate memory for file stack!\n");
    break;
    default:
      printf("Error! Unexpected error\n");
    break;
  }

  return error;
}