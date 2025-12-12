#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <string.h>
#include <math.h>

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
      vcut = 1;   
      hcut = segments;    
      break;
      
    case HORIZONTAL_CUT:
      vcut = segments;    
      hcut = 1;           
      break;
      
    case GRID_CUT:
      for (hcut = (int)sqrt(segments); hcut > 0; hcut--) {
        if (segments % hcut == 0) {
          vcut = segments / hcut;
          break;
        }
      }
      if (hcut == 0) {
        hcut = 1;
        vcut = segments;
      }
      break;
      
    default:
      printf("Unknown cut type: %d\n", cutType);
      return;
  }

  int partWidth = fileInfo->imgWidth / hcut;
  int partHeight = fileInfo->imgHeight / vcut;
  
  int srcRowSize = ((fileInfo->imgWidth * 3 + 3) / 4) * 4;
  int dstRowSize = ((partWidth * 3 + 3) / 4) * 4;        
  
  bmpHeader *newHeader = copyHeader(fileHeader);
  bmpInfo *newInfo = copyInfo(fileInfo);
  
  newInfo->imgWidth = partWidth;
  newInfo->imgHeight = partHeight;
  newInfo->dataSize = dstRowSize * partHeight;
  newHeader->fileSize = newInfo->dataSize + sizeof(bmpHeader) + sizeof(bmpInfo);
  
  unsigned char *rowBuffer = (unsigned char*)malloc(dstRowSize);
  if (!rowBuffer) {
    printf("Error: Memory allocation failed\n");
    free(newHeader);
    free(newInfo);
    return;
  }
  
  for (int rowPart = 0; rowPart < vcut; rowPart++) {
    for (int colPart = 0; colPart < hcut; colPart++) {
      char fn[64] = {0};
      int partIndex = rowPart * hcut + colPart;
      snprintf(fn, sizeof(fn), "out/%d.bmp", partIndex);
      
      FILE *newFile = fopen(fn, "wb");
      if (!newFile) {
        printf("Error: Cannot create file %s\n", fn);
        free(rowBuffer);
        free(newHeader);
        free(newInfo);
        return;
      }
      
      fwrite(newHeader, sizeof(bmpHeader), 1, newFile);
      fwrite(newInfo, sizeof(bmpInfo), 1, newFile);
      

      int startY = rowPart * partHeight;
      int startX = colPart * partWidth;
      
      for (int y = startY + partHeight - 1; y >= startY; y--) {
        memset(rowBuffer, 0, dstRowSize);

        for (int x = 0; x < partWidth; x++) {
          int srcX = startX + x;
          rowBuffer[x * 3] = img[y][srcX][0];    
          rowBuffer[x * 3 + 1] = img[y][srcX][1]; 
          rowBuffer[x * 3 + 2] = img[y][srcX][2]; 
        }
        
        fwrite(rowBuffer, dstRowSize, 1, newFile);
      }
      
      fclose(newFile);
      printf("Created: %s\n", fn);
    }
  }
  
  free(rowBuffer);
  free(newHeader);
  free(newInfo);
}