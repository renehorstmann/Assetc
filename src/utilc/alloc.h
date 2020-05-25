#ifndef UTILC_ALLOC_H
#define UTILC_ALLOC_H

#include <stdlib.h>
#include <assert.h>

/** calls malloc and asserts the given pointer to be not NULL */
static void *assert_malloc(size_t n, size_t size) {
    void *mem = malloc(n * size);
    assert(mem && "malloc failed, got a null ptr");
    return mem;
}

/** calls calloc and asserts the given pointer to be not NULL */
static void *assert_calloc(size_t n, size_t size) {
    void *mem = calloc(n, size);
    assert(mem && "calloc failed, got a null ptr");
    return mem;
}

/** calls realloc and asserts the given pointer to be not NULL */
static void *assert_realloc(void *mem, size_t n, size_t size) {
    void *mem_new = realloc(mem, n * size);
    assert(mem_new && "realloc failed, got a null ptr");
    return mem_new;
}

/** calls free and sets the pointer NULL (must be a pointer to the address) */
static void free0(void **mem) {
    free(*mem);
    *mem = NULL;
}

/** calls assert_malloc and casts to the given type */
#define New(type, n) (type *) assert_malloc((n), sizeof(type))

/** calls assert_calloc and casts to the given type */
#define New0(type, n) (type *) assert_calloc((n), sizeof(type))

/** calls assert_realloc and casts to the given type */
#define ReNew(type, mem, n) (type *) assert_realloc((mem), (n), sizeof(type))


/** calls malloc and casts to the given type */
#define TryNew(type, n) (type *) malloc((n) * sizeof(type))

/** calls calloc and casts to the given type */
#define TryNew0(type, n) (type *) calloc((n), sizeof(type))

/** calls realloc and casts to the given type */
#define TryReNew(type, mem, n) (type *) realloc((mem), (n) * sizeof(type))

/** wrappper for free0 */
#define Free0(mem) free0((void **)&(mem))

#endif //UTILC_ALLOC_H

