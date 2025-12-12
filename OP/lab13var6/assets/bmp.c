#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

bmpHeader* readHeader(FILE *fp)
{
  bmpHeader *out = (bmpHeader*)malloc(sizeof(bmpHeader));
  if (!out)
    return NULL;

  fread(out, sizeof(bmpHeader), 1, fp);

  return out;
}

bmpInfo* readInfo(FILE *fp)
{
  bmpInfo *out = (bmpInfo*)malloc(sizeof(bmpInfo));
  if (!out)
    return NULL;

  fread(out, sizeof(bmpInfo), 1, fp);

  return out;
}

void cutBmp(bmpHeader *fileHeader, bmpInfo *fileInfo, FILE *fp)
{

}