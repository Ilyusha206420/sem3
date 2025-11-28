#define Y
#if !defined(Z) && (0 || defined(Y))
int result = 1;
#else
int result = 0;
#endif
