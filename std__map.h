#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#pragma once

typedef enum { RED, BLACK } Color;

#define std__map_prototype(key_type, value_type) \
    typedef struct key_type##_##value_type##_node { \
        key_type *key; \
        value_type *value; \
        Color color; \
        struct key_type##_##value_type##_node *left, *right, *parent; \
    } key_type##_##value_type##_node; \
    typedef struct key_type##_##value_type##_map { \
        key_type##_##value_type##_node *root; \
        int size; \
        int (*cmp)(key_type *a, key_type *b); \
        void (*free_key)(key_type *); \
        void (*free_value)(value_type *); \
        key_type##_##value_type##_node *(*new_node)(key_type *key, value_type *value); \
        void (*__left_rotate)(struct key_type##_##value_type##_map *map, key_type##_##value_type##_node *x); \
        void (*__right_rotate)(struct key_type##_##value_type##_map *map, key_type##_##value_type##_node *y); \
        void (*__fix_insert)(struct key_type##_##value_type##_map *map, key_type##_##value_type##_node *z); \
        void (*insert)(struct key_type##_##value_type##_map *map, key_type *key, value_type *value); \
        value_type *(*get)(struct key_type##_##value_type##_map *map, key_type *key); \
        void (*free_node)(struct key_type##_##value_type##_map *map, key_type##_##value_type##_node *node); \
        void (*free)(struct key_type##_##value_type##_map *map); \
        void (*display)(struct key_type##_##value_type##_map *map); \
    }  key_type##_##value_type##_map; \
    static key_type##_##value_type##_map *key_type##_##value_type##_new_map(int (*cmp)(key_type *a, key_type *b)); \
    static key_type##_##value_type##_node *key_type##_##value_type##_new_node(key_type *key, value_type *value); \
    static void key_type##_##value_type##_left_rotate(key_type##_##value_type##_map *map, key_type##_##value_type##_node *x); \
    static void key_type##_##value_type##_right_rotate(key_type##_##value_type##_map *map, key_type##_##value_type##_node *y); \
    static void key_type##_##value_type##_fix_insert(key_type##_##value_type##_map *map, key_type##_##value_type##_node *z); \
    static void key_type##_##value_type##_insert(key_type##_##value_type##_map *map, key_type *key, value_type *value); \
    static value_type *key_type##_##value_type##_get(key_type##_##value_type##_map *map, key_type *key); \
    static void key_type##_##value_type##_free_node(key_type##_##value_type##_map *map, key_type##_##value_type##_node *node); \
    static void key_type##_##value_type##_free(key_type##_##value_type##_map *map); \
    static void key_type##_##value_type##_display(key_type##_##value_type##_map *map); \


#define std__map_implementation(key_type, value_type) \
    static void key_type##_##value_type##_left_rotate(key_type##_##value_type##_map *map, key_type##_##value_type##_node *x) { \
        key_type##_##value_type##_node *y = x->right; \
        x->right = y->left; \
        if (y->left) y->left->parent = x; \
        y->parent = x->parent; \
        if (!x->parent) map->root = y; \
        else if (x == x->parent->left) x->parent->left = y; \
        else x->parent->right = y; \
        y->left = x; \
        x->parent = y; \
    } \
    static void key_type##_##value_type##_right_rotate(key_type##_##value_type##_map *map, key_type##_##value_type##_node *y) { \
        key_type##_##value_type##_node *x = y->left; \
        y->left = x->right; \
        if (x->right) x->right->parent = y; \
        x->parent = y->parent; \
        if (!y->parent) map->root = x; \
        else if (y == y->parent->left) y->parent->left = x; \
        else y->parent->right = x; \
        x->right = y; \
        y->parent = x; \
    } \
    static void key_type##_##value_type##_fix_insert(key_type##_##value_type##_map *map, key_type##_##value_type##_node *z) { \
        while (z->parent && z->parent->color == RED) { \
            if (z->parent == z->parent->parent->left) { \
                key_type##_##value_type##_node *y = z->parent->parent->right; \
                if (y && y->color == RED) { \
                    z->parent->color = BLACK; \
                    y->color = BLACK; \
                    z->parent->parent->color = RED; \
                    z = z->parent->parent; \
                } else { \
                    if (z == z->parent->right) { \
                        z = z->parent; \
                        key_type##_##value_type##_left_rotate(map, z); \
                    } \
                    z->parent->color = BLACK; \
                    z->parent->parent->color = RED; \
                    key_type##_##value_type##_right_rotate(map, z->parent->parent); \
                } \
            } else { \
                key_type##_##value_type##_node *y = z->parent->parent->left; \
                if (y && y->color == RED) { \
                    z->parent->color = BLACK; \
                    y->color = BLACK; \
                    z->parent->parent->color = RED; \
                    z = z->parent->parent; \
                } else { \
                    if (z == z->parent->left) { \
                        z = z->parent; \
                        key_type##_##value_type##_right_rotate(map, z); \
                    } \
                    z->parent->color = BLACK; \
                    z->parent->parent->color = RED; \
                    key_type##_##value_type##_left_rotate(map, z->parent->parent); \
                } \
            } \
        } \
        map->root->color = BLACK; \
    } \
    static void key_type##_##value_type##_insert(key_type##_##value_type##_map *map, key_type *key, value_type *value) { \
        key_type##_##value_type##_node *z = key_type##_##value_type##_new_node(key, value); \
        key_type##_##value_type##_node *y = NULL; \
        key_type##_##value_type##_node *x = map->root; \
        while (x) { \
            y = x; \
            int cmp = map->cmp(z->key, x->key); \
            if (cmp == 0) { \
                if (map->free_key) map->free_key(x->key); \
                if (map->free_value) map->free_value(x->value); \
                x->key = key; \
                x->value = value; \
                free(z); \
                return; \
            } else if (cmp < 0) { \
                x = x->left; \
            } else { \
                x = x->right; \
            } \
        } \
        z->parent = y; \
        if (!y) map->root = z; \
        else if (map->cmp(z->key, y->key) < 0) { \
            y->left = z; \
        } else { \
            y->right = z; \
        } \
        key_type##_##value_type##_fix_insert(map, z); \
        map->size++; \
    } \
    static value_type *key_type##_##value_type##_get(key_type##_##value_type##_map *map, key_type *key) { \
        key_type##_##value_type##_node *node = map->root; \
        while (node) { \
            int cmp = map->cmp(key, node->key); \
            if (cmp == 0) { \
                return node->value; \
            } else if (cmp < 0) { \
                node = node->left; \
            } else { \
                node = node->right; \
            } \
        } \
        return NULL; \
    } \
    static void key_type##_##value_type##_free_node(key_type##_##value_type##_map *map, key_type##_##value_type##_node *node) { \
        if (!node) return; \
        key_type##_##value_type##_free_node(map, node->left); \
        key_type##_##value_type##_free_node(map, node->right); \
        if (map->free_key) map->free_key(node->key); \
        if (map->free_value) map->free_value(node->value); \
        free(node); \
    } \
    static void key_type##_##value_type##_free(key_type##_##value_type##_map *map) { \
        key_type##_##value_type##_free_node(map, map->root); \
        free(map); \
    } \
    static key_type##_##value_type##_node *key_type##_##value_type##_new_node(key_type *key, value_type *value) { \
        key_type##_##value_type##_node *node = malloc(sizeof(key_type##_##value_type##_node)); \
        node->key = key; \
        node->value = value; \
        node->color = RED; \
        node->left = node->right = node->parent = NULL; \
        return node; \
    } \
    static void key_type##_##value_type##_display(key_type##_##value_type##_map *map) { \
        if (!map->root) { \
            printf("Map is empty\n"); \
            return; \
        } \
        printf("Map size: %d\n", map->size); \
        printf("Root key: %d, value: %d\n", *(int *)map->root->key, *(int *)map->root->value); \
    } \
    static key_type##_##value_type##_map *key_type##_##value_type##_new_map(int (*cmp)(key_type *a, key_type *b)) { \
        key_type##_##value_type##_map *map = malloc(sizeof(key_type##_##value_type##_map)); \
        map->root = NULL; \
        map->size = 0; \
        map->cmp = cmp; \
        map->free_key = NULL; \
        map->free_value = NULL; \
        map->new_node = key_type##_##value_type##_new_node; \
        map->__left_rotate = key_type##_##value_type##_left_rotate; \
        map->__right_rotate = key_type##_##value_type##_right_rotate; \
        map->__fix_insert = key_type##_##value_type##_fix_insert; \
        map->insert = key_type##_##value_type##_insert; \
        map->get = key_type##_##value_type##_get; \
        map->free_node = key_type##_##value_type##_free_node; \
        map->free = key_type##_##value_type##_free; \
        map->display = key_type##_##value_type##_display; \
        return map; \
    } \


#define init_std__map(key_type, value_type) \
    std__map_prototype(key_type, value_type) \
    std__map_implementation(key_type, value_type) \

#define std__map(key_type, value_type) key_type##_##value_type##_map
#define new_std__map(key_type, value_type) key_type##_##value_type##_new_map
#define delete_std__map(key_type, value_type) key_type##_##value_type##_free

