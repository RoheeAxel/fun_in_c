#include <stdarg.h>
#include "../auto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct methods_s {
    char *name;
    void (*__function__)(void *self, ...);
} methods_t;

typedef struct object_s {
    char *name;
    methods_t *__methods__;
    size_t __methods_count__;
    char **__super__;
    size_t __super_count__;
} object_t;

object_t **__objects__;
size_t __objects_count__;

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

void setSuper(object_t *object, ...) {
    object->__super_count__ = 0; \
    va_list args;
    va_start(args, object);
    while (1) {
        char *arg = va_arg(args, char *);
        if (arg == NULL) {
            break;
        }
        printf("%s\n", arg);
        object->__super__ = realloc(object->__super__, sizeof(char *) * (object->__super_count__ + 1));
        object->__super__[object->__super_count__] = arg;
        object->__super_count__++;
        printf("%p\n", object);
    }
    printf("%ld\n", object->__super_count__);
    va_end(args);
}

#define class(class_name, body, ...) \
    if (__objects_count__ == 0) { \
        __objects__ = malloc(sizeof(object_t *)); \
        __objects__[0] = malloc(sizeof(object_t)); \
        __objects__[0]->name = #class_name; \
        __objects__[0]->__methods_count__ = 0; \
        __objects__[0]->__methods__ = malloc(sizeof(methods_t)); \
        setSuper(__objects__[0], __VA_ARGS__, NULL); \
        printf("%p\n", __objects__[0]); \
        printf("%ld\n", __objects__[0]->__super_count__); \
        __objects__[0]->__super__ = 0; \
        __objects_count__++; \
    } else { \
        __objects__ = realloc(__objects__, sizeof(object_t) * (__objects_count__ + 1)); \
        __objects__[__objects_count__]->name = #class_name; \
        __objects__[__objects_count__]->__methods_count__ = 0; \
        __objects__[__objects_count__]->__methods__ = malloc(sizeof(methods_t)); \
        setSuper(__objects__[__objects_count__], __VA_ARGS__, NULL); \
    printf("%ld\n", __objects__[__objects_count__]->__super_count__); \
        __objects_count__++; \
    } \
    class_struct(class_name, body) \
    void __dtor__##class_name(class_name *self) { \
        free(self); \
    } \

#define addMethod(classname, method_name, return_type, body) \
    ({ \
        return_type classname##_##method_name body; \
        classname##_##method_name; \
        size_t index_object = 0; \
        for (int i=0; i<__objects_count__; i++) { \
            if (strcmp(__objects__[i]->name, #classname) == 0) { \
                index_object = i; \
                break; \
            } \
        } \
        if (__objects__[index_object]->__methods_count__ == 0) { \
            __objects__[index_object]->__methods__ = malloc(sizeof(methods_t)); \
            __objects__[index_object]->__methods__[0].__function__ = classname##_##method_name; \
            __objects__[index_object]->__methods__[0].name = n_strcat(#classname, #method_name); \
            __objects__[index_object]->__methods_count__++; \
        } else { \
            __objects__[index_object]->__methods__ = realloc(__objects__[index_object]->__methods__, sizeof(methods_t) * (__objects__[index_object]->__methods_count__ + 1)); \
            __objects__[index_object]->__methods__[__objects__[index_object]->__methods_count__].__function__ = classname##_##method_name; \
            __objects__[index_object]->__methods__[__objects__[index_object]->__methods_count__].name = n_strcat(#classname, #method_name); \
            __objects__[index_object]->__methods_count__++; \
        } \
    })

#define method(classname, method_name, object, ...) \
    ({size_t index_object = 0; \
    bool found = false; \
    for (int i=0; i<__objects_count__; i++) { \
        if (strcmp(__objects__[i]->name, #classname) == 0) { \
            index_object = i; \
            break; \
        } \
    } \
    for (int i=0; i<__objects__[index_object]->__methods_count__; i++) { \
        if (strcmp(__objects__[index_object]->__methods__[i].name, n_strcat(#classname, #method_name)) == 0) { \
            __objects__[index_object]->__methods__[i].__function__((void *)((classname *)&object), ##__VA_ARGS__); \
            found = true; \
        } \
    } \
    if (!found) { \
        printf("not found searching in super\n"); \
        for (int i=0; i<__objects__[index_object]->__super_count__; i++) { \
            printf("Hi there\n"); \
            for (int j=0; j<__objects_count__; j++) { \
                if (strcmp(__objects__[j]->name, __objects__[index_object]->__super__[i]) == 0) { \
                    for (int k=0; k<__objects__[j]->__methods_count__; k++) { \
                        if (strcmp(__objects__[j]->__methods__[k].name, n_strcat(__objects__[index_object]->__super__[i], #method_name)) == 0) { \
                            __objects__[j]->__methods__[k].__function__((void *)((classname *)&object), ##__VA_ARGS__); \
                            found = true; \
                        } \
                    } \
                } \
            } \
        } \
    } \
    })

#define new(classname, ...) \
    {__VA_ARGS__, __dtor__##classname}