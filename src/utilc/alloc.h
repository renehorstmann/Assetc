#ifndef UTILC_ALLOC_H
#define UTILC_ALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/** calls malloc and raises sig if it fails */
static void *raising_malloc(size_t n, size_t size, int sig) {
    void *mem = malloc(n * size);
    if(!mem) {
        fprintf(stderr, "malloc failed with n: %zu size: %zu\n", n, size);
        raise(sig);
    }
    return mem;
}

/** calls calloc and raises sig if it fails */
static void *raising_calloc(size_t n, size_t size, int sig) {
    void *mem = calloc(n, size);
    if(!mem) {
        fprintf(stderr, "calloc failed with n: %zu size: %zu\n", n, size);
        raise(sig);
    }
    return mem;
}

/** calls realloc and raises sig if it fails */
static void *raising_realloc(void *mem, size_t n, size_t size, int sig) {
    void *mem_new = realloc(mem, n * size);
    if(!mem) {
        fprintf(stderr, "realloc failed with n: %zu size: %zu\n", n, size);
        raise(sig);
    }
    return mem_new;
}

/** calls free and sets the pointer NULL (must be a pointer to the address) */
static void free0(void **mem) {
    free(*mem);
    *mem = NULL;
}

/** calls raising_malloc and casts to the given type */
#define New(type, n) (type *) raising_malloc((n), sizeof(type), SIGABRT)

/** calls raising_calloc and casts to the given type */
#define New0(type, n) (type *) raising_calloc((n), sizeof(type), SIGABRT)

/** calls raising_realloc and casts to the given type */
#define ReNew(type, mem, n) (type *) raising_realloc((mem), (n), sizeof(type), SIGABRT)


/** calls malloc and casts to the given type */
#define TryNew(type, n) (type *) malloc((n) * sizeof(type))

/** calls calloc and casts to the given type */
#define TryNew0(type, n) (type *) calloc((n), sizeof(type))

/** calls realloc and casts to the given type */
#define TryReNew(type, mem, n) (type *) realloc((mem), (n) * sizeof(type))

/** wrappper for free0 */
#define Free0(mem) free0((void **)&(mem))

#endif //UTILC_ALLOC_H

