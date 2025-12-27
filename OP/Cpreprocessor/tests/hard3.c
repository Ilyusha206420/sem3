// hard3.c - nested conditionals, redefinition, modulo and logical not
#define VAL 10
#if (VAL - 5) * 2 == 10 && !defined(FLAG)
int hard3 = VAL;
#else
int hard3 = 0;
#endif

#undef VAL
#define VAL 20
#if VAL > 15 && (VAL % 3 == 2)
int hard3b = VAL;
#else
int hard3b = -1;
#endif

