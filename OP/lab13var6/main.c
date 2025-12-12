#include "assets/parser.h"
#include "assets/bmp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("\033[91mError! Wrong argument\n\033[39m");
    return -1;
  }
  FILE *fp = NULL;
  int segments = DEFAULT_SEGMENT_COUNT;
  int cutType = DEFAULT_CUT_TYPE;
  int err = parseConfig(argv[1], &fp, &segments, &cutType);
  switch (err) {
    case 0:
    break;

    case ERROR_CONF_OPEN:
      printf("\033[91mError! Cant open file : %s\n\033[39m", argv[1]);
      return -1;
    break;

    case ERROR_CONF_SYNTAX:
      printf("\033[91mError! Wrong syntax in config file!\n\033[39m");
      if (fp) 
        fclose(fp);
      return -1;
    break;

    case ERROR_CUT_TYPE:
      printf("\033[91mError! Unknown cut type!\n\033[39m");
      if (fp) 
        fclose(fp);
      return -1;
    break;

    case ERROR_FILE_COUNT:
      printf("\033[91mError! You must add only one file in config!\n\033[39m");
      if (fp) 
        fclose(fp);
      return -1;
    break;

    case ERROR_OPEN_FILE:
      printf("\033[91mError! Cant open BMP-file!\n\033[39m");
      return -1;
    break;

    default:
      printf("\033[91mUnexpected error : %d!\n\033[39m", err);
      if (fp) 
        fclose(fp);
      return -1;  
    break;
  }
  if (!fp) {
    printf("\033[91mError! There is not filename in config file\n\033[39m");
    return -1;
  }

  bmpHeader *head = readHeader(fp);
  bmpInfo *info = readInfo(fp);
  char ***img = (char***)malloc(sizeof(char**) * info->imgHeight);
  for (int i = info->imgHeight; i >= 0; --i) {
    img[i] = (char**)malloc(sizeof(char*) * info->imgWidth);
    for (int n = 0; n < info->imgWidth; n++) {
      img[i][n] = (char*)malloc(sizeof(char) * 3);
      fread(img[i][n], 3, 1, fp);
    }
  }
  fclose(fp);

  cutBmp(head, info, img, cutType, segments);

  return 0;
}