#include <stdio.h>

#include <stdlib.h>
#include <x86intrin.h>
#pragma GCC target("avx2")


#include "true_string.h"

typedef float vec __attribute__ (( vector_size(32) ));

vec *alloc(int size) {
    vec *ptr = (vec *)aligned_alloc(32, size * sizeof(vec));
    memset(ptr, 0, size * sizeof(vec));
    return ptr;
}

#define init_matrix(type)   \
    typedef struct matrix_##type { \
        char *__class__; \
        char *__type__; \
        type *data; \
        int width; \
        int height; \
        struct matrix_##type *(*__mul__)(struct matrix_##type *self, struct matrix_##type *other); \
        struct matrix_##type *(*__imul__)(struct matrix_##type *self, struct matrix_##type *other); \
        const str (*__repr__)(struct matrix_##type *self); \
        void (*set)(struct matrix_##type *self, int **data); \
    } matrix_##type; \
    matrix_##type *matrix_##type##_ctor(int width, int height); \
    void matrix_##type##_set(matrix_##type *self, int **data) { \
        for (int i = 0; i < self->height; i++) { \
            for (int j = 0; j < self->width; j++) { \
                self->data[i * self->width + j] = data[i][j]; \
            } \
        } \
    } \
    const str matrix_##type##_repr(matrix_##type *self) { \
        str dims = String("["); \
        IADD(dims, String("]")); \
        str result = String("Matrix<"); \
        str _type = String(self->__type__); \
        IADD(result, _type); \
        IADD(result, String(">")); \
        IADD(result, dims); \
        return result; \
    } \
    matrix_##type *matrix_##type##_mul(matrix_##type *self, matrix_##type *other) { \
        int nB = (self->width + 7) / 8; \
        matrix_##type *result = matrix_##type##_ctor(self->width, other->height); \
        vec *_self = alloc(self->height * self->width); \
        vec *transpose = alloc(other->width * other->height); \
        for (int i = 0; i < other->width; i++) {\
            for (int j = 0; j < other->width; j++) { \
                _self[i * nB + j / 8][j % 8] = self->data[i * self->width + j]; \
                transpose[i * nB + j / 8][j % 8] = other->data[j * other->width + i]; \
            } \
        } \
        for (int i = 0; i < self-> height; i++) { \
            for (int j = 0; j < self->width; j++) { \
                vec s = {0}; \
                for (int k = 0; k < nB; k++) { \
                    s += _self[i * nB + k] * transpose[j * nB + k]; \
                } \
                for (int k = 0; k < 8; k++) { \
                    result->data[i * self->width + j] += s[k]; \
                } \
            } \
        } \
        return result; \
    } \
    matrix_##type *matrix_##type##_imul(matrix_##type *self, matrix_##type *other) { \
        for (int i = 0; i < self->height; i++) { \
            for (int j = 0; j < self->width; j++) { \
                self->data[i * self->width + j] = 0; \
                for (int k = 0; k < self->width; k++) { \
                    self->data[i * self->width + j] += self->data[i * self->width + k] * other->data[k * self->width + j]; \
                } \
            } \
        } \
        return self; \
    } \
    matrix_##type *matrix_##type##_ctor(int width, int height) { \
        matrix_##type *m = (matrix_##type *)malloc(sizeof(matrix_##type)); \
        m->__class__ = "Matrix"; \
        m->__type__ = #type; \
        m->width = width; \
        m->height = height; \
        m->__mul__ = matrix_##type##_mul; \
        m->__imul__ = matrix_##type##_imul; \
        m->__repr__ = matrix_##type##_repr; \
        m->set = matrix_##type##_set; \
        m->data = (type *)malloc(width * height * sizeof(type)); \
    } \


#define Matrix(type) matrix_##type##_ctor
#define matrix(type) matrix_##type*


init_matrix(int)
init_matrix(float)

int main()
{
    alloc(10);
    int matrix_size = 3;
    matrix(int) my_matrix = Matrix(int)(matrix_size, matrix_size);
    matrix(float) my_matrix_f = Matrix(float)(matrix_size, matrix_size);
    int **data = (int **)malloc(matrix_size * sizeof(int *));
    for (int i = 0; i < matrix_size; i++) {
        data[i] = (int *)malloc(matrix_size * sizeof(int));
        for (int j = 0; j < matrix_size; j++) {
            data[i][j] = i + j;
            // printf("%d ", data[i][j]);
        }
        // printf("\n");
    }
    my_matrix->set(my_matrix, data);
    matrix(int) my_matrix2 = Matrix(int)(matrix_size, matrix_size);
    my_matrix2->set(my_matrix2, data);
    matrix(int) result = MULT(my_matrix, my_matrix2);


    print(result);
    str s = String("Hello, ");
    print(s);
    str s2 = String(5);
    print(s2);
    str s3 = IADD(s, s2);
    str s4 = String(3.5);
    // printf("%s\n", s4);
    print(s4);
    print(s3);
}

#undef TYPE