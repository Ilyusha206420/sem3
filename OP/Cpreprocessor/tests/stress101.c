// stress101.c - long chain of elifs and alternating defines
#define X1 1
#define X2 2
#define X3 3
#define X4 4

#if 0
  int out = 0;
#elif (X1 + X2 + X3 + X4) == 10
  int out = 10;
#elif defined(NONE) && (1)
  int out = 20;
#elif ( (X4*X3 - X2) + (X1*X1) ) == 13
  int out = 30;
#else
  int out = -1;
#endif

// nested alternation
#if defined(X1)
  #if (X2 * (X3 + X4) - 5) > 0
    int nested = 1;
  #else
    int nested = 0;
  #endif
#endif

// many sequential small conditionals
#if (X1 == 1)
  int seq_a = 1;
#endif
#if (X2 == 2)
  int seq_b = 2;
#endif
#if (X3 == 3)
  int seq_c = 3;
#endif
