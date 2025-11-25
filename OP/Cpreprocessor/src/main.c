#include "assets/hashMap.h"

int main(int argc, char **argv)
{
  HashMap* test = HMinit(1024);
  HMfree(test);
  return 0;
}