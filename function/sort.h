
#include "../auto.h"
#include <stddef.h>
#include <stdbool.h>

#define __sort__(sort_function, array) \
    ({ \
    for (size_t __i__ = 0; __i__ < sizeof(array) / sizeof(array[0]); __i__++) \
        for (size_t __j__ = 0; __j__ < sizeof(array) / sizeof(array[0]); __j__++) \
            if (sort_function(array[__i__], array[__j__])) { \
                auto tmp = array[__i__]; \
                array[__i__] = array[__j__]; \
                array[__j__] = tmp; \
            } \
    })

#define sort __sort__

