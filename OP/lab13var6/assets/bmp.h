#pragma once

#include <stdio.h>

#pragma pack(2)

typedef struct
{
  unsigned short fileType;
  unsigned int  fileSize;
  unsigned short reserved1;
  unsigned short reserved2;
  unsigned int  offset;
} bmpHeader;


typedef struct
{
  unsigned int   infoSize;
  unsigned int   imgWidth;
  unsigned int   imgHeight;
  unsigned short imgLayers;
  unsigned short bitsPerDot;
  unsigned int   compType;
  unsigned int   dataSize;
  unsigned int   XpixelsPerMeter;
  unsigned int   YpixelsPerMeter;
  unsigned int   colorUsed;
  unsigned int   colorImportant;
} bmpInfo;

#pragma pack()

bmpHeader* readHeader(FILE *fp);
bmpInfo* readInfo(FILE *fp);

void cutBmp(bmpHeader *fileHeader, bmpInfo *fileInfo, char ***img, int cutType, int segments);