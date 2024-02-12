#include <setjmp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#define yield(func, n) \
    if (! setjmp(func##_gen_jmp)) {  \
        func##_ret = n;                    \
        longjmp(func##_caller_jmp, 1);     \
    }

#define generator(ret, func, T, argv)        \
  static jmp_buf func##_caller_jmp;             \
  static jmp_buf func##_gen_jmp;                \
  static bool func##_continue=false;            \
  static ret func##_ret;                        \
  bool func##__real(T argv);                 \
  ret func(T argv) {                         \
    if (!func##_continue) {                     \
      func##_continue=true ;                    \
      if (! setjmp(func##_caller_jmp)) {        \
        func##__real(argv);                     \
      } else {                                  \
        return func##_ret;                      \
      }                                         \
    }                                           \
     else {                                     \
      longjmp(func##_gen_jmp,1);                \
    }                                           \
    return 0;                                   \
  }                                             \
  bool func##__real(T argv)


#define run_until_end(func, ...) \
({ \
while (func(__VA_ARGS__)); \
})
