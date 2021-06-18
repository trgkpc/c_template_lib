#include <stdio.h>

#define DEBUG_ON
#define LIB_DEBUG_OFF
#define PRINT_DEBUG_OFF
#define NUP_ERROR_OFF

#define WARNING(...)

#ifdef LIB_DEBUG_ON
#define LIB_DEBUG_LOG(...) printf("LIB:"__VA_ARGS__)
#else
#define LIB_DEBUG_LOG(...)
#endif

#ifdef DEBUG_ON
#define DEBUG_LOG(...) printf("DEBUG_MSG:"__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#ifdef PRINT_DEBUG_ON
#define PRINT_DEBUG_LOG(...) printf("PRINT:"__VA_ARGS__)
#else
#define PRINT_DEBUG_LOG(...)
#endif

#ifdef NUP_ERROR_ON
#define NUP(...) printf("NUP function not defined...\n");
#else
#define NUP(...)
#endif
