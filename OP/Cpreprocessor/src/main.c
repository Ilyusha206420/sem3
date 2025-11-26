#include "assets/hashMap.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  HashMap *test = HMinit(3);
  HMadd(test, "testKey", "testVal");
  HMadd(test, "secondKey", "secondVal");
  HMadd(test, "thirdKey", "thirdVal");
  HMadd(test, "fourthKey", "fourthVal");
  HMadd(test, "testKey", "newTestVal");
  HMadd(test, "fifthKey", "");

  for (int i = 0; i < test->numberOfPockets; i++) {
    printf("[%d]   ", i);
    HMPnode *node = test->map[i].begin;
    while (node) {
      printf("-[key: %s | val: %s]", node->key, node->val);
      node = node->next;
    }
    printf("\n");
  }
  return 0;
}