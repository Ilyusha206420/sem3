// stress103.c - toggling macros and complex boolean expressions
#define P 2
#define Q 3

#if (P*Q > 5) && ( (Q-P) < 5 )
  int t0 = 1;
  #if defined(P) && !defined(ZZZ)
    int t1 = 2;
  #endif
#else
  int t0 = 0;
#endif

// additional nested mixing of !defined and arithmetic
#if !defined(MAGIC) && ( (P+Q)*(P-Q) >= -5 )
  int t2 = 3;
  #if ( (P+P+P+P) == 8 ) || defined(XXX)
    int t3 = 4;
  #endif
#endif
