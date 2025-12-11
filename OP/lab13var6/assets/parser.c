#include "parser.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseConfig(char *confName, FILE **fp, int *segments, int *cutType)
{
  FILE *conf = fopen(confName, "r");
  if (!conf)
    return ERROR_CONF_OPEN;

  char buf[16] = {0};
  char ch = EOF;
  int i = 0;
  int seg = 0;
  char *filename = NULL;
  while (ch = getc(conf), ch != EOF && i < 16) {
    if (ch == ' ' || ch == ':') {
      if (!strcmp(buf, "filename")) {
        if (filename)
          return ERROR_FILE_COUNT;
        while (ch = getc(conf), ch == ':' || ch == ' ')
          if (ch == '\n' || ch == EOF) return ERROR_CONF_SYNTAX;
        
        long pos = ftell(conf);

        while (ch = getc(conf), ch != ' ' && ch != '\n' && ch != EOF);
        
        long size = ftell(conf) - pos + 1;
        filename = (char*)malloc(sizeof(char) * size);

        fseek(conf, pos - 1, SEEK_SET);
        for (long i = 0; i < size - 1; i++)
          filename[i] = getc(conf);
        filename[size - 1] = '\0';
        *fp = fopen(filename, "rb");
        free(filename);
        if (!*fp)
          return ERROR_OPEN_FILE;
      }
      else if (!strcmp(buf, "segments")) {
        while (ch = getc(conf), ch == ':' || ch == ' ') 
          if (ch == '\n' || ch == EOF) return ERROR_CONF_SYNTAX;
        while (ch != EOF && ch != '\n' && ch != ' ') {
          seg *= 10;
          seg += ch - '0';
          ch = getc(conf);
        }
        if (ch != '\n')
          while (ch = getc(conf), ch != EOF && ch != '\n');
      }
      else if (!strcmp(buf, "type")) {
        while (ch = getc(conf), ch == ':' || ch == ' ')
          if (ch == '\n' || ch == EOF) return ERROR_CONF_SYNTAX;
        switch (ch) {
        case 'g':
          *cutType = GRID_CUT;
          break;
        case 'h':
          *cutType = HORIZONTAL_CUT;
          break;
        case 'v':
          *cutType = VERTICAL_CUT;
          break;
        default:
          return ERROR_CUT_TYPE;
        }
        if (ch != '\n') while (ch = getc(conf), ch != EOF && ch != '\n');
      }
      else  
        return ERROR_CONF_SYNTAX;
      
      for (int n = 0; n < 16; n++)
        buf[n] = '\0';
      i = 0;
    }
    if (ch != '\n') buf[i++] = ch;
  }
  if (i == 16)
    return ERROR_CONF_SYNTAX;
  if (seg)
    *segments = seg;

  return 0;
}