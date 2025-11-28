// stress104.c - many small defines and conditionals accumulating state
#define B0 0
#define B1 1
#define B2 2
#define B3 3
#define B4 4

int acc = 0;
#if defined(B0)
  acc += B0;
#endif
#if defined(B1)
  acc += B1;
#endif
#if defined(B2)
  acc += B2;
#endif
#if defined(B3)
  acc += B3;
#endif
#if defined(B4)
  acc += B4;
#endif

// check accumulation
#if acc == 10
  int acc_ok = 1;
#else
  int acc_ok = 0;
#endif
