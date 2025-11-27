#include "preprocessor.h"

int main(int argc, char **argv)
{
  processFile("src/preprocessor.c", "processed.c");
  return 0;
}