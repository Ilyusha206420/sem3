// prec_check6.c - defined() vs numeric macro expansion
#define X 10
#if defined(X)
int d = 1;
#else
int d = 0;
#endif
