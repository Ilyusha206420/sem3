#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>

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

bmpHeader* copyHeader(bmpHeader *header)
{
  bmpHeader *out = (bmpHeader*)malloc(sizeof(bmpHeader));
  out->fileType = header->fileType;
  out->fileSize = header->fileSize;
  out->reserved1 = header->reserved1;
  out->reserved2 = header->reserved2;
  out->offset = header->offset;
  return out;
}

bmpInfo* copyInfo(bmpInfo *info)
{
  bmpInfo *out = (bmpInfo*)malloc(sizeof(bmpInfo));
  out->infoSize = info->infoSize;
  out->imgWidth = info->imgWidth;
  out->imgHeight = info->imgHeight;
  out->imgLayers = info->imgLayers;
  out->bitsPerDot = info->bitsPerDot;
  out->compType = info->compType;
  out->dataSize = info->dataSize;
  out->XpixelsPerMeter = info->XpixelsPerMeter;
  out->YpixelsPerMeter = info->YpixelsPerMeter;
  out->colorUsed = info->colorUsed;
  out->colorImportant = info->colorImportant;
  return out;
}

void cutBmp(bmpHeader *fileHeader, bmpInfo *fileInfo, char ***img, int cutType, int segments)
{
  int vcut = 0;
  int hcut = 0;
  switch (cutType) {
  case VERTICAL_CUT:
    vcut = segments;
  break;
  case HORIZONTAL_CUT:
    hcut = segments;
  break;
  case GRID_CUT:
    for (; hcut < ((int)(segments / 2) + 1) && segments % hcut != 0; hcut++);
    if (segments % hcut != 0)
      return;
    vcut = segments / hcut;
  break;
  default:
    return;
  break;
  }
  bmpHeader *newHeader = copyHeader(fileHeader);
  bmpInfo *newInfo = copyInfo(fileInfo);
  newInfo->imgHeight /= (vcut + 1);
  newInfo->imgWidth /= (hcut + 1);
  int hoff = 0;
  int voff = 0;
  char fn[64] = {0};
  FILE *newFile = NULL;
  for (int i = 0; i < segments; i++) {
    snprintf(fn, sizeof(fn), "out/%d.bmp", i);
    printf("%s", fn);
    newFile = fopen(fn, "wb");
    fwrite(newHeader, sizeof(bmpHeader), 1, newFile);
    fwrite(newInfo, sizeof(bmpInfo), 1, newFile);
    fclose(newFile);
  }
}