#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STRING_H
#define STRING_H

#define str str_t*

typedef struct str_t {
    char* __class__;
    char* __type__;
    int size;
    char *data;
    const struct str (*__repr__)(struct str self);
    int (*__len__)(struct str self);
    struct str (*__iadd__)(struct str self, struct str other);
} str_t;


#define WARNING(msg) fprintf(stderr, "Warning: %s\n", msg)
str string_ctor();
str _string_from_char(char *data);
str _string_from_int(int data);

#define String(x) _Generic((x), \
    char *: _string_from_char, \
    const char *: _string_from_char, \
    int: _string_from_int, \
    default: string_ctor \
)(x)


#define print(s) if (s != NULL && s->__repr__ != NULL && s->data != NULL) { printf("%s\n", s->__repr__(s)->data); } else { WARNING("Cannot print NULL object"); }

#define typeof(x) _Generic((x), \
    int: "int", \
    float: "float", \
    double: "double", \
    char: "char", \
    char *: "char *", \
    default: "unknown type" \
)


#define len(s) s->__len__(s)

#define ADD(a, b) a->__add__(a, b)

#define IADD(a, b)  a->__iadd__(a, b)

#define MULT(a, b) a->__mul__(a, b)

#define IMULT(a, b) a->__imul__(a, b)



#endif // STRING_H