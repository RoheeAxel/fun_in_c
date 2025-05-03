#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#pragma once


#define check_args(a, ...) \
    va_list args; \
    va_start(args, a); \
    void *arg; \
    size_t i = 0; \
    while ((arg = va_arg(args, void *)) != NULL) { \
        i++; \
    } \
    if (i != 0) { \
        printf("Error: %s\n", "Invalid number of arguments"); \
        return NULL; \
    } \

#define vector_prototype(type, name) \
    typedef struct type##_vector{ \
        type **Data; \
        int Size; \
        int Capacity; \
        type **(*get)(struct type##_vector *this); \
        int (*get_size)(struct type##_vector *this); \
        void (*push_back)(struct type##_vector *this, type *value); \
        void (*push_front)(struct type##_vector *this, type *value); \
        type *(*pop_front)(struct type##_vector *this); \
        type *(*pop_back)(struct type##_vector *this); \
        type *(*at)(struct type##_vector *this, int index); \
        void (*merge)(struct type##_vector *this, struct type##_vector *other); \
        size_t (*__get_type_size)(__attribute__((unused)) struct type##_vector *this); \
        struct type##_vector *(*reverse)(struct type##_vector *this); \
    } type##_vector; \
    static type##_vector *type##_vector_new(int b, ...); \
    static void type##_##name##_free(type##_vector *this); \
    static type **type##_vector_get(type##_vector *this); \
    static int type##_vector_get_size(type##_vector *this); \
    static void type##_vector_push_back(type##_vector *this, type *value); \
    static void type##_vector_push_front(type##_vector *this, type *value); \
    static void type##_vector_merge(type##_vector *this, type##_vector *other); \
    static size_t type##_vector__get_type_size(__attribute__((unused))type##_vector *this); \
    static type *type##_vector_pop_back(type##_vector *this); \
    static type *type##_vector_pop_front(type##_vector *this); \
    static type *type##_vector_at(type##_vector *this, int index); \

#define vector_implementation(type) \
    static type **type##_vector_get(type##_vector *this){ \
        return this->Data; \
    } \
    static int type##_vector_get_size(type##_vector *this){ \
        return this->Size; \
    }   \
    static type *type##_vector_at(type##_vector *this, int index){ \
        if (index < 0) { \
            return this->Data[this->Size + index]; \
        } \
        return this->Data[index]; \
    } \
    static void type##_vector_push_back(type##_vector *this, type *value){ \
        if (this->Size == this->Capacity) { \
            this->Capacity *= 2; \
            this->Data = realloc(this->Data, this->Capacity * sizeof(type *)); \
        } \
        this->Data[this->Size] = malloc(sizeof(type)); \
        memcpy(this->Data[this->Size], value, this->__get_type_size(this)); \
        this->Size++; \
    } \
    static type *type##_vector_pop_front(type##_vector *this){ \
        type *value = this->Data[0]; \
        for (int i = 0; i < this->Size - 1; i++) { \
            this->Data[i] = this->Data[i + 1]; \
        } \
        this->Size--; \
        return value; \
    } \
    static void type##_vector_push_front(type##_vector *this, type *value){ \
        if (this->Size == this->Capacity) { \
            this->Capacity *= 2; \
            this->Data = realloc(this->Data, this->Capacity * sizeof(type *)); \
        } \
        for (int i = this->Size; i > 0; i--) { \
            this->Data[i] = this->Data[i - 1]; \
        } \
        this->Data[0] = malloc(sizeof(type)); \
        memcpy(this->Data[0], value, this->__get_type_size(this)); \
        this->Size++; \
    } \
    static type *type##_vector_pop_back(type##_vector *this){ \
        type *value = this->Data[this->Size - 1]; \
        this->Size--; \
        return value; \
    } \
    static type##_vector *type##_vector_reverse(type##_vector *this){ \
        type##_vector *reversed = newT(vector, type, 0); \
        for (int i = this->Size - 1; i >= 0; i--) { \
            reversed->push_back(reversed, this->at(this, i)); \
        } \
        return reversed; \
    } \
    static void type##_vector_merge(type##_vector *this, type##_vector *other){ \
        for (int i = 0; i < other->Size; i++) { \
            this->push_back(this, other->at(other, i)); \
        } \
    } \
    static size_t type##_vector__get_type_size(__attribute__((unused))type##_vector *this){ \
        return sizeof(type); \
    } \
    static type##_vector *type##_vector_new(int size, ...){ \
        check_args(size); \
        type##_vector *this = malloc(sizeof(type##_vector)); \
        this->Data = calloc(size + 1, sizeof(type *)); \
        this->Size = size; \
        this->Capacity = size + 1; \
        this->get = type##_vector_get; \
        this->get_size = type##_vector_get_size; \
        this->push_back = type##_vector_push_back; \
        this->push_front = type##_vector_push_front; \
        this->pop_front = type##_vector_pop_front; \
        this->pop_back = type##_vector_pop_back; \
        this->reverse = type##_vector_reverse; \
        this->merge = type##_vector_merge; \
        this->at = type##_vector_at; \
        this->__get_type_size = type##_vector__get_type_size; \
        return this; \
    } \
    static void type##_vector_free(type##_vector *shared){ \
        for (int i = 0; i < shared->Size; i++) { \
            free(shared->Data[i]); \
        } \
        free(shared->Data); \
        free(shared); \
    } \


#define init_vector(type) \
    vector_prototype(type, vector) \
    vector_implementation(type) \



    #define map(map_function, type, self) \
    ({ \
        type##_vector *new_vector = newT(vector, type, self->get_size(self)); \
        for (int i = 0; i < self->get_size(self); i++) { \
            new_vector->push_back(new_vector, map_function(self->at(self, i))); \
        } \
        new_vector; \
    }) \

// #define delete(name, type, this) type##_##name##_free(this)
#define newT(name, type, ...) type##_##name##_new(__VA_ARGS__, NULL) // New template
#define new(name, ...) name##_new(__VA_ARGS__, NULL) // New
#define delete(name, ...) name##_free(__VA_ARGS__) // Delete
#define deleteT(name, type, ...) type##_##name##_free(__VA_ARGS__) // Delete template
#define len(name) name->get_size(name) // Length
#define value_at(name, index) name->at(name, index) // At
#define vector(type) type##_vector
