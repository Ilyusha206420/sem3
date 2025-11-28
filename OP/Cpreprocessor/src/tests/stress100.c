// stress100.c - endurance stress test for the preprocessor (~100 lines)
// Exercises nested #if/#elif/#else/#endif, many defined() checks,
// macro redefinition/undef, complex arithmetic and modulo, and deep nesting.

// Base macros
#define M0 0
#define M1 1
#define M2 2
#define M3 3
#define M4 4
#define M5 5
#define M6 6
#define M7 7
#define M8 8
#define M9 9

// Start complex conditionals
#if defined(M1) && (M2 + M3 * (M4 - 1) == 2 + 3 * 3)
  // level 1 true
  int s0 = 10;
  #if (M9 % 2 == 1) && ( (M8 - M7) * (M6 + M5) >= 0 )
    // level 2 true
    int s1 = 11;
    #if 0
      // unreachable block should be skipped entirely
      int unreachable = 999;
    #elif defined(FOO) && (1 + 1 == 3)
      int branchFoo = -1;
    #elif defined(M0) || ( (100 / (5*5)) == 4 )
      // this elif is true due to defined(M0)
      int s2 = 12;
    #else
      int s2 = -12;
    #endif

    // deeper nested mixing arithmetic and defined checks
    #if ((M5 * M4 + M3) - (M2 + M1) > 0) || defined(BAR)
      int s3 = 13;
      #if ( ( ( (M1 + M2) * (M3 + M4) ) % 7 ) != 0 )
        int s4 = 14;
      #elif defined(M9) && ( (50 - 3*13) <= 11 )
        int s4 = 15;
      #else
        int s4 = -14;
      #endif
    #endif

  #else
    int s1 = -11;
  #endif

#elif (M0 + M1 + M2 + M3 + M4 + M5 + M6 + M7 + M8 + M9) > 100
  int s0 = 1000;
#else
  int s0 = -10;
#endif

// Redefinition and undef tests
#define VAL 42
#if VAL == 42
  int valA = VAL;
#endif

#undef VAL
#define VAL (21*2)
#if VAL / 7 == 6 && (VAL % 5 == 2)
  int valB = VAL;
#else
  int valB = -1;
#endif

// Chains of elif to stress branch tracking
#if 0
  int chain = 0;
#elif 0
  int chain = 1;
#elif (1+1 == 3)
  int chain = 2;
#elif defined(M3) && ( (M4 * M5 - 10) == 10 )
  int chain = 3;
#elif ( (M2 + M2 + M2 + M2 + M2 + M2 + M2 + M2 + M2 + M2) == 20 )
  int chain = 4;
#elif defined(NONEXIST) && (9999)
  int chain = 5;
#else
  int chain = 6;
#endif

// Very deep nesting to test stack depth handling
#if 1
  #if 1
    #if 1
      #if 1
        #if 1
          #if 1
            #if 1
              #if 1
                #if 1
                  int deep = 123;
                #endif
              #endif
            #endif
          #endif
        #endif
      #endif
    #endif
  #endif
#endif

// Mixed complex expression combining &&, || and !
#define A1 5
#define A2 10
#if (A1 * 3 > A2) || ( (A2 - A1) % 5 == 0 && !defined(FOOBAR) )
  int mix = 1;
#else
  int mix = 0;
#endif

// Many small conditionals in sequence to accumulate branches
#if defined(M1)
  int seq0 = 0;
#endif
#if defined(M2)
  int seq1 = 1;
#endif
#if defined(M3)
  int seq2 = 2;
#endif
#if defined(M4)
  int seq3 = 3;
#endif
#if defined(M5)
  int seq4 = 4;
#endif

// Final check with a big arithmetic expression
#if (M0 + M1 * (M2 + M3) - (M4 * M5) + (M6 * (M7 + M8)) / (M9 + 1) ) < 1000
  int final_ok = 1;
#else
  int final_ok = 0;
#endif

// End of stress100.c
