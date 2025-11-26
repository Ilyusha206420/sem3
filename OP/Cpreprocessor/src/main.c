#include "preprocessor.h"

int main(int argc, char **argv)
{
  processFile("test.c", "processed.c");
  return 0;
}