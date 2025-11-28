#define A 1
#ifdef A
int x = A;
#endif
#undef A
#ifdef A
int y = 2;
#endif
