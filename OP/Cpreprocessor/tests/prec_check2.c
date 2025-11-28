// prec_check2.c - same but with parentheses to disambiguate
#define X 10
#if (X == 10) && 1
int ok2 = 1;
#else
int ok2 = 0;
#endif
