#include <stdio.h>

int main(void);
int hello(void);
int sum(int a, int b);

int main(void) {
    hello();
    return 0;
}

int hello(void) {
    printf("Hello\n");
    return 0;
}