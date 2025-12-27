// prec_check5.c - check macro equality without extra &&
#define X 10
#if X == 10
int z = 1;
#else
int z = 0;
#endif
