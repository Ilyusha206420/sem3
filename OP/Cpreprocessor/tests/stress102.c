#define A 7
#if A < 5
  int r = -1;
#elif A == 7
  int r = 7;
#elif A > 10
  int r = 10;
#else
  int r = 0;
#endif

#undef A
#define A (3+4)
#if (A*2) == 14 && defined(A)
  int r2 = A;
#endif

#if 0
  int ladder = 0;
#elif 0
  int ladder = 1;
#elif 0
  int ladder = 2;
#elif (1+2+3+4+5+6+7+8+9) == 45
  int ladder = 45;
#else
  int ladder = -999;
#endif

#define mult(x, y) (x * y)
int m()
{
  return mult(A, 2);
}