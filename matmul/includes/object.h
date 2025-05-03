typedef struct object_t {
    char *__class__;
    char *__type__;
    int (*__len__)(struct object_t *self);
    struct object_t *(*__add__)(struct object_t *self, struct object_t *other);
    struct object_t *(*__iadd__)(struct object_t *self, struct object_t *other);
    struct object_t *(*__mul__)(struct object_t *self, struct object_t *other);
    struct object_t *(*__imul__)(struct object_t *self, struct object_t *other);
} object_t;
