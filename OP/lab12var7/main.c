#include "myLab.h"
#include <stdio.h>

int main(int argc, char **argv) 
{
  if(argc == 1) {
    printf("Error! Nothing to do\n");
    return -1;
  }
  if(argc != 5) {
    printf("Error! Wrong arguments\n");
    return -1;
  }
  deleteWords(argv[1], 3, argv[2], argv[3], argv[4]);
  printf("%s\n", argv[1]);
  return 0;
}