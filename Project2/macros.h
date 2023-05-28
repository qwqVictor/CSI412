#ifndef VICTORHUANG_PROJ2_MACROS
#define VICTORHUANG_PROJ2_MACROS
#include <stdlib.h>
#include <stdio.h>

#define _try(x)                                                                                    \
    {                                                                                              \
        int _try_ret;                                                                              \
        if ((_try_ret = (x))) {                                                                    \
            fprintf(stderr, "Error: expr '" #x "' in line %d returned %d.\n", __LINE__, _try_ret); \
            exit(_try_ret);                                                                        \
        }                                                                                          \
    }
#define _try_do(x, c)                                                                              \
    {                                                                                              \
        int _try_ret;                                                                              \
        if ((_try_ret = (x))) {                                                                    \
            fprintf(stderr, "Error: expr '" #x "' in line %d returned %d.\n", __LINE__, _try_ret); \
        }                                                                                          \
        else {                                                                                     \
            c;                                                                                     \
        }                                                                                          \
    }

#ifdef DEBUG
#define debug(template, ...) (fprintf(stderr, "[DEBUG]%s:%d| " template "\n", __FILE__, __LINE__, ##__VA_ARGS__))
#else
#define debug(template, ...) (__VA_ARGS__)
#endif
#endif // !VICTORHUANG_PROJ2_MACROS