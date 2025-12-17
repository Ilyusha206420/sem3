#define N1 1000
#define N2 500
#define N3 250

#if (N1 - N2) > (N3 * 2)
  int big = 1;
#elif (N2 + N3) == 750
  int big = 2;
#else
  int big = 0;
#endif

#undef N3
#define N3 333
#if ( (N1 + N2 + N3) % 3 ) == 0
  int big2 = 1;
#else
  int big2 = 0;
#endif

#if 0
  int many = 0;
#elif 0
  int many = 1;
#elif 0
  int many = 2;
#elif 0
  int many = 3;
#elif 1
  int many = 4;
#endif
