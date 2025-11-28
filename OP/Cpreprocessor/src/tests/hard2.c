// hard2.c - nested #if / #elif branches with defined checks and arithmetic
#define X 0
#if 0
#error unreachable
#elif defined(Y) && (5 * 5 == 25)
int hard2 = 1;
#elif defined(X) || (2 + 2 * 2 == 6)
int hard2 = 2;
#else
int hard2 = 3;
#endif

