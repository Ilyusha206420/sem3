// prec_expr.c - test compound expression with macro and defined()
#define VAL 10
#if (VAL - 5) * 2 == 10 && !defined(FLAG)
int r = 1;
#else
int r = 0;
#endif
