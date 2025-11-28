// hard1.c - tricky defined() + arithmetic/logic test
#define A 1
#if defined(A) && (3 + 4 * 2 > 10)
int hard1 = 1;
#else
int hard1 = 0;
#endif

