// prec_check1.c - check equality vs && precedence
#define X 10
#if X == 10 && 1
int ok1 = 1;
#else
int ok1 = 0;
#endif
