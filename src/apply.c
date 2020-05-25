#include "utilc/dynarray.h"
#include "utilc/strviu.h"
#include "generate.h"
#include "apply.h"

DynArray(char, StrArray)

static void push_strviu(StrArray *array, strviu sv) {
    StrArray_push_array(array, sv.begin, sv_length(sv));
}

static void push_string(StrArray *array, const char *str) {
    StrArray_push_array(array, str, strlen(str));
}

char *apply_template(const char *template, const char *name, const File *files, size_t files_size) {
    StrArray res = DynArray_INIT;
    strviu tmp = ToStrViu(template);

    strviu next = sv_eat_until_cstring(tmp, "@include_asset_h");
    tmp.end = next.begin;
    next.begin += strlen("@include_asset_h");

    push_strviu(&res, tmp);
    push_string(&res, "#include \"");
    push_string(&res, name);
    push_string(&res, ".h\"\n");


    tmp = next;
    next = sv_eat_until_cstring(tmp, "@file_init");
    tmp.end = next.begin;
    next.begin += strlen("@file_init");

    push_strviu(&res, tmp);
    for(int i=0; i < files_size; i++) {
        char *init = generate_file_init_on_heap(i, files[i].name, files[i].data, files[i].size);
        push_string(&res, init);
        free(init);
    }

    tmp = next;
    next = sv_eat_until_cstring(tmp, "@list_init");
    tmp.end = next.begin;
    next.begin += strlen("@list_init");

    push_strviu(&res, tmp);
    for(int i=0; i < files_size; i++) {
        char *init = generate_list_init_on_heap(i);
        push_string(&res, init);
        free(init);
    }

    tmp = next;
    next = sv_eat_until_cstring(tmp, "@map_init");
    tmp.end = next.begin;
    next.begin += strlen("@map_init");

    push_strviu(&res, tmp);
    for(int i=0; i < files_size; i++) {
        char *init = generate_map_init_on_heap(i);
        push_string(&res, init);
        free(init);
    }

    push_strviu(&res, next);

    return res.array;
}