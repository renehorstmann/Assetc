#include "utilc/alloc.h"
#include "generate.h"

const char *generate_file_init_on_heap(int id, const char *name, const char *data, size_t size) {
    return NULL;
}

const char *generate_list_init_on_heap(int id) {
    size_t len = 0;
    char *str = New(char, len);

    return str;
}

const char *generate_map_init_on_heap(int id) {
    return NULL;
}
