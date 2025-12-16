#if 1
  #if 1
    #if 1
      #if 1
        #if 1
          #if 1
            #if 1
              #if 1
                #if 1
                  int r = 10;
                #endif
              #endif
            #endif
          #endif
        #endif
      #endif
      int r2 = 0;
    #endif
  #endif
#endif

#define ALALA
#ifdef ALALA
int ifd = 1;
#else
int ifd = 0;
#endif

#undef ALALA
#ifndef ALALA
  int indef = 1;
#else
  int indef = 0;
#endif
