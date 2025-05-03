#include <stdio.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef try_catch
static jmp_buf buf;
int error_code = 0;
char *what = NULL;
typedef struct where_t {
    char *file;
    const char *function;
    int line;
} where_t;

typedef struct exception_t {
    int error_code;
    char *what;
    where_t *where;
} exception_t;

where_t *where = NULL;
exception_t *exception = NULL;

#define ERROR_DIVISION_BY_ZERO 1
#define ERROR_OTHER 2

char *messages[3] = {
    "No error",
    "Division by zero",
    "Other error"
};

void basic_throw(int error)
{
    what = messages[error];
    error_code = error;
    exception->what = messages[error];
    exception->error_code = error;
    longjmp(buf, error);
}

void advance_throw(int error, char *message)
{
    what = message;
    error_code = error,
    exception->what = message;
    exception->error_code = error; \
    longjmp(buf, error);
}

void splitter(int __error_code__, ...)
    {
        va_list args;
        va_start(args, __error_code__);
        void *arg;
        char *msg;
        size_t nb_args = 0;
        while ((arg = va_arg(args, char*)) != NULL) {
            msg = arg;
            nb_args++;
        }
        if (nb_args == 0) {
            basic_throw(__error_code__);
        }
        else if (nb_args == 1) {
            advance_throw(__error_code__, msg);
        }
        else {
            fprintf(stderr, "Too many arguments for throw\n");
            exit(1);
        }
        va_end(args);
}
#define __throw__(ERROR_CODE, ...) \
    splitter(ERROR_CODE, __VA_ARGS__, NULL); \

#define throw(ERROR_CODE, ...) \
    where = (where_t *) malloc(sizeof(where_t)); \
    where->file = __FILE__; \
    where->function = __func__; \
    where->line = __LINE__; \
    exception = (exception_t *) malloc(sizeof(exception_t)); \
    exception->error_code = ERROR_CODE; \
    exception->where = where; \
    __throw__(ERROR_CODE, __VA_ARGS__, NULL) \

#define try if (!setjmp(buf))
#define catch(error_code) else if (setjmp(buf) == (error_code))
#define ERROR(error_code) error_code, messages[error_code]
#endif