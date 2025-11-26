#include "preprocessor.h"

#include <stdio.h>

FILE* openFile(char *fileName, char *mode);

int processFile(char *input, char *output)
{
  FILE *ifp = openFile(input, "r");
  if (!ifp)
    return FILE_OPEN_ERROR;
  
  FILE *tmp = openFile("/tmp/myPreprocessorTmp", "w");
  if (!tmp)
    return TMP_FILE_CREATE_ERROR;

  

  return 0;
}

FILE* openFile(char *fileName, char *mode)
{
  FILE *out = NULL;
  int try = 0;
  while (try++, !out && try < 10000)
    out = fopen(fileName, mode);
  return out;
}