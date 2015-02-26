#ifndef CCT_COMMON_INCLUDES_H
#define CCT_COMMON_INCLUDES_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define cct_alloc(type, count) ((type*)(malloc(sizeof(type) * (count))))

#ifndef CCT_DUMMY_TYPE
typedef float* dummy_type; // DUMMY_TYPE_DECLARATION
#else
typedef DUMMY_TYPE dummy_type;
#endif

#endif
