#ifndef UTILC_HASHMAP_H
#define UTILC_HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Creates a HashMap with the name (name) and the types for key and value (key_type) (value_type).
 * The function hashfunction should take a key and return an unsigned hash number.
 * The function keycmp_function should return true, if both key parameters are equal.
 * The function keycpy_function should copy the input key into the return value.
 * The function keykill_function should free the given key.
 * 
 * Construct a Map with <name> map; <name>_new(&map, approx_size);.
 * Kill it with <name>_kill.
 * <name>_get returns an pointer to a value for the given key (if not available yet, it will create a new entry).
 * <name>_remove removes the value under the given key, if available.
 *
 * Get a map iterator with <name>_get_iter(&map);
 */
#define HashMap(name, key_type, value_type, hash_function, keycmp_function, keycpy_function, keykill_function)\
typedef struct name ## _Item {\
    key_type key;\
    value_type value;\
    struct name ## _Item *next;\
} name ## _Item;\
typedef struct name {\
    name ## _Item **map;\
    int map_size;\
} name;\
typedef struct name ## _Iter {\
    name *hashmap;\
    int hash_row;\
    name ## _Item *next;\
} name ## _Iter;\
void name ## _new(name *self, int approx_size) {\
    self->map = (name ## _Item **) calloc(approx_size, sizeof(name ## _Item *));\
    self->map_size = approx_size;\
}\
void name ## _kill(name  *self) {\
    for (int i = 0; i < self->map_size; i++) {\
        name ## _Item *item = self->map[i];\
        while (item) {\
            name ## _Item *next = item->next;\
            keykill_function(item->key);\
            free(item);\
            item = next;\
        }\
    }\
    free(self->map);\
    self->map = NULL;\
    self->map_size = 0;\
}\
value_type *name ## _get(const name *self, key_type key) {\
    unsigned hash = hash_function(key) % self->map_size;\
    name ## _Item **item = &self->map[hash];\
    while (*item && !keycmp_function(key, (*item)->key))\
        item = &(*item)->next;\
    if (!*item) {\
        *item = (name ## _Item *) malloc(sizeof(name ## _Item));\
        (*item)->key = keycpy_function(key);\
        (*item)->next = NULL;\
    }\
    return &(*item)->value;\
}\
void name ## _remove(const name *self, key_type key) {\
    unsigned hash = hash_function(key) % self->map_size;\
    name ## _Item **item = &self->map[hash];\
    while (*item && !keycmp_function(key, (*item)->key))\
        item = &(*item)->next;\
    if (*item) {\
        name ## _Item *kill = *item;\
        *item = (*item)->next;\
        keykill_function(kill->key);\
        free(kill);\
    }\
}\
name ## _Iter name ## _get_iter(name *self) {\
    return (name ## _Iter) {self, -1, NULL};\
}\
name ## _Item *name ## _Iter_next(name ## _Iter *it) {\
    name ## _Item *ret;\
    if(it->next) {\
        ret = it->next;\
        it->next = ret->next;\
        return ret;\
    }\
    do {\
        it->hash_row++;\
        if(it->hash_row >= it->hashmap->map_size)\
            return NULL;\
        ret = it->hashmap->map[it->hash_row];\
    } while(ret == NULL);\
    it->next = ret->next;\
    return ret;\
}

/** impl function for StrHashMap */
static unsigned StrHashMap_impl_hash(const char *key) {
    unsigned hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
    return hash;
}

/** impl function for StrHashMap */
static bool StrHashMap_impl_keycmp(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

/** impl function for StrHashMap */
static const char *StrHashMap_impl_keycpy(const char *cpy) {
    char *result = (char *) malloc(strlen(cpy) + 1);
    strcpy(result, cpy);
    return result;
}

/** impl function for StrHashMap */
static void StrHashMap_impl_keykill(const char *key) {
    free((void *) key);
}


/**
 * Creates a string HashMap with the name (name) and the type for value (value_type).
 * 
 * Construct a Map with <name> map; <name>_new(&map, approx_size);.
 * Kill it with <name>_kill.
 * <name>_get returns an pointer to a value for the given string key (if not available yet, it will create a new entry).
 * <name>_remove removes the value under the given string key, if available.
 */
#define StrHashMap(name, value_type) HashMap(name, const char *, value_type,\
StrHashMap_impl_hash, StrHashMap_impl_keycmp, StrHashMap_impl_keycpy, StrHashMap_impl_keykill)


#endif //UTILC_HASHMAP_H

