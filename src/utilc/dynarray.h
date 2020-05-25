#ifndef UTILC_DYNARRAY_H
#define UTILC_DYNARRAY_H

#include <stdlib.h>

#define DynArray_INIT {0}

/**
 * Creates an dynamic array of type (type) with the typename name.
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_push copies an item to the end of the array (copies to the last element).
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 * <name>_pop returns a copy of the last item and removes it from the array.
 */
#define DynArray(type, name) \
typedef struct name { \
    type *array; \
    size_t capacity; \
    size_t size; \
} name; \
static void name ## _kill(name *self) { \
    free(self->array); \
    self->array = NULL; \
    self->capacity = 0; \
    self->size = 0; \
} \
static void name ## _set_capacity(name *self, size_t capacity) { \
    type *new_array = (type *) realloc(self->array, capacity * sizeof(type)); \
    if (new_array) { \
        self->array = new_array; \
        self->capacity = capacity; \
        if (self->size > capacity) \
            self->size = capacity; \
    } \
} \
static void name ## _resize(name *self, size_t size) { \
    if (size > self->capacity) { \
        name ## _set_capacity(self, size * 2); \
        if(size>self->capacity) \
            /* alloc error */ \
            return;\
    } \
    self->size = size; \
} \
static void name ## _push(name *self, type item) { \
    name ## _resize(self, self->size+1); \
    self->array[self->size-1] = item; \
} \
static void name ## _push_array(name *self, const type *item_array, size_t n) { \
    name ## _resize(self, self->size+n); \
    for(int i=0; i<n; i++)\
        self->array[self->size-n+i] = item_array[i]; \
} \
static type *name ## _append(name *self) { \
    name ## _resize(self, self->size+1); \
    return &self->array[self->size-1]; \
} \
static type name ## _pop(name *self) { \
    if(self->size > 0) \
        return self->array[--self->size]; \
    return (type) {0}; \
}




/**
 * Creates an dynamic array of type (type) with the typename name.
 * This version dont uses copies (push and pop). For use with array types like int[2].
 *
 * Construct it as empty struct ( = {};, or calloc).
 * Kill it with <name>_kill;
 * <name>_set_capacity sets the internal capacity. If realloc fails, nothing changes.
 * <name>_resize sets the array size and may set capacity with size*2.
 * <name>_append creates an item at the end and returns its pointer (returns the last element).
 */
#define DynArrayWithoutCopy(type, name) \
typedef struct name { \
    type *array; \
    size_t capacity; \
    size_t size; \
} name; \
static void name ## _kill(name *self) { \
    free(self->array); \
    self->array = NULL; \
    self->capacity = 0; \
    self->size = 0; \
} \
static void name ## _set_capacity(name *self, size_t capacity) { \
    type *new_array = (type *) realloc(self->array, capacity * sizeof(type)); \
    if (new_array) { \
        self->array = new_array; \
        self->capacity = capacity; \
        if (self->size > capacity) \
            self->size = capacity; \
    } \
} \
static void name ## _resize(name *self, size_t size) { \
    if (size > self->capacity) { \
        name ## _set_capacity(self, size * 2); \
        if(size>self->capacity) \
            /* alloc error */ \
            return;\
    } \
    self->size = size; \
} \
static type *name ## _append(name *self) { \
    name ## _resize(self, self->size+1); \
    return &self->array[self->size-1]; \
}

#endif //UTILC_DYNARRAY_H

