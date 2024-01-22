#include <stdarg.h>
#include "../auto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **__class_names__;
int __class_names_count__;

//__method__ is an array of function pointers
void (**__methods__)(void *, ...);
char **__method_names__;
int __methods_count__;

char *n_strcat(char *dest, char *src) {
    char *result = malloc(sizeof(char) * (strlen(dest) + strlen(src) + 2));
    strcpy(result, dest);
    strcat(result, "_");
    strcat(result, src);
    return result;
}
#define class_struct(name, body) \
    typedef struct name##_t { \
        body \
        void (*__dtor__)(struct name *self); \
    } name;

#define class(name, body) \
    if (__class_names_count__ == 0) { \
        __class_names__ = malloc(sizeof(char *)); \
        __class_names__[0] = #name; \
        __class_names_count__++; \
    } else { \
        __class_names__ = realloc(__class_names__, sizeof(char *) * (__class_names_count__ + 1)); \
        __class_names__[__class_names_count__] = #name; \
        __class_names_count__++; \
    } \
    class_struct(name, body) \
    void __dtor__##name(name *self) { \
        free(self); \
    } \

#define addMethod(classname, method_name, return_type, body) \
    ({ \
        return_type classname##_##method_name body; \
        classname##_##method_name; \
        if (__methods_count__ == 0) { \
            __methods__ = malloc(sizeof(void *)); \
            __methods__[0] = classname##_##method_name; \
            char *method_name_str = n_strcat(#classname, #method_name); \
            __method_names__ = malloc(sizeof(char *)); \
            __method_names__[0] = method_name_str; \
            __methods_count__++; \
        } else { \
            __methods__ = realloc(__methods__, sizeof(void *) * (__methods_count__ + 1)); \
            __methods__[__methods_count__] = classname##_##method_name; \
            char *method_name_str = n_strcat(#classname, #method_name); \
            __method_names__ = realloc(__method_names__, sizeof(char *) * (__methods_count__ + 1)); \
            __method_names__[__methods_count__] = method_name_str; \
            __methods_count__++; \
        } \
    })

#define method(classname, method_name, object, ...) \
    classname *ptr = &object; \
    void *cast_object = ptr; \
    for (int i=0; i<__methods_count__; i++) { \
        if (strcmp(__method_names__[i], n_strcat(#classname, #method_name)) == 0) { \
            __methods__[i](cast_object, ##__VA_ARGS__); \
        } \
    } \

#define new(classname, ...) \
    {__VA_ARGS__, __dtor__##classname}

