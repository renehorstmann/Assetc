#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utilc/dynarray.h"
#include "generate.h"

DynArray(char, StrArray)

static void push_string(StrArray *array, const char *str) {
    StrArray_push_array(array, str, strlen(str));
}

char *generate_file_init_on_heap(int id, const char *name, const char *data, size_t size) {
    StrArray str = DynArray_INIT;

    char id_str[12]; // max length for an 32 bit int
    sprintf(id_str, "%d", id);

    push_string(&str, "static const char name_");
    push_string(&str, id_str);
    push_string(&str, "[] = \"");
    push_string(&str, name);
    push_string(&str, "\";\n");

    push_string(&str, "static const char data_");
    push_string(&str, id_str);
    push_string(&str, "[] =\n        \"");

    for(int i=0; i<size; i++) {
        char hex[5];
        sprintf(hex, "\\x%02hhx", data[i]);
        push_string(&str, hex);
    }

    push_string(&str, "\";\n");

    StrArray_push(&str, '\0');
    return str.array;
}

char *generate_list_init_on_heap(int id) {
    StrArray str = DynArray_INIT;

    char id_str[12]; // max length for an 32 bit int
    sprintf(id_str, "%d", id);

    push_string(&str, "    static llist list_");
    push_string(&str, id_str);
    push_string(&str, " = LLIST_INIT(name_");
    push_string(&str, id_str);
    push_string(&str, ", data_");
    push_string(&str, id_str);
    push_string(&str, ");\n");

    StrArray_push(&str, '\0');
    return str.array;
}

char *generate_map_init_on_heap(int id) {
    StrArray str = DynArray_INIT;

    char id_str[12]; // max length for an 32 bit int
    sprintf(id_str, "%d", id);

    push_string(&str,  "    map_add(&list_");
    push_string(&str, id_str);
    push_string(&str, ", name_");
    push_string(&str, id_str);
    push_string(&str, ");\n");

    StrArray_push(&str, '\0');
    return str.array;
}
