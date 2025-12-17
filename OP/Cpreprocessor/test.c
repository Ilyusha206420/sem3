#define MASK 0xFF48
#define SHIFT 8
#define VERSION 2
#define FEATURE_X_ENABLED 1

#if (MASK >> SHIFT) == 0xFF && VERSION * 10 > 15
    #define CONFIG_A 1
#elif (MASK & 0x00FF) == 0 && FEATURE_X_ENABLED
    #define CONFIG_A 2
#else
    #define CONFIG_A 3
#endif

#if __LINE__ % 2 == 0
    #define EVEN_LINE 1
#else
    #define EVEN_LINE 0
#endif

#if 1 << 2 == 4 && (3 + 5) * 2 > 10
    #define MATH_OK 1
#endif

#define VALUE1 100
#define VALUE2 VALUE1 + 50
#define VALUE3 VALUE2 * 2

#undef VALUE1
#define VALUE1 200

#define VALUE4 VALUE3 - VALUE1

#define TEMP X
#define X 1
#undef TEMP
#define TEMP Y
#define Y X + 1

#define ALPHA BETA + 10
#define BETA GAMMA * 2
#define GAMMA 5

#if defined(__DATE__) && defined(__TIME__)
    #define BUILD_INFO_AVAILABLE 1
#endif

#if __STDC__
    #define C_STANDARD 1
#endif

#line 1000 "special_file.c"

#define LINE_NUM __LINE__
#define FILE_NAME __FILE__

#define EMPTY_MACRO
#define SPACES      
#define TAB_MACRO	

#define LOOKS_LIKE_EXPR (a + b)
#define ANOTHER one + two * three

#define SPECIAL_CHARS "{}\\[]();,.*&^%$#@!~`|<>?/"

#define VERY_LONG_MACRO_NAME_THAT_SHOULD_BE_HANDLED_PROPERLY 42
#define ANOTHER_VERY_LONG_NAME_FOR_TESTING_PURPOSES_1234567890 "test"

#define int INT_TYPE
#define return RETURN_VALUE
#define if IF_CONDITION
