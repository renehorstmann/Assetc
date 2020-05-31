#include <ctype.h>
#include <stdio.h>

#include "utilc/dynarray.h"
#include "utilc/strviu.h"
#include "generate.h"
#include "apply.h"

DynArray(char, StrArray)


static void push_string(StrArray *array, const char *str) {
    StrArray_push_array(array, str, strlen(str));
}

static void push_string_as_upper(StrArray *array, const char *str) {
    while (*str)
        StrArray_push(array, (char) toupper(*str++));
}


char *apply_header(const char *template, const char *file_name, const char *namespace) {
    strviu tmp = ToStrViu(template);
    char *res;
    char *to_free = NULL;

    // include guard
    {
        StrArray ig = DynArray_INIT;
        push_string(&ig, "#ifndef ASSETC_");
        if (namespace) {
            push_string_as_upper(&ig, namespace);
            StrArray_push(&ig, '_');
        }
        push_string_as_upper(&ig, file_name);
        push_string(&ig, "_H\n#define ASSETC_");
        if (namespace) {
            push_string_as_upper(&ig, namespace);
            StrArray_push(&ig, '_');
        }
        push_string_as_upper(&ig, file_name);
        push_string(&ig, "_H\n");
        StrArray_push(&ig, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@include_quard@", ig.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&ig);
    }

    // asset with namespace
    {
        StrArray name = DynArray_INIT;
        if (namespace) {
            push_string(&name, namespace);
            StrArray_push(&name, '_');
        }
        push_string(&name, "asset");
        StrArray_push(&name, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@asset@", name.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&name);
    }

    return res;
}

char *apply_source(const char *template, const char *file_name, const char *namespace,
                   const File *files, size_t files_size) {
    strviu tmp = ToStrViu(template);
    char *res;
    char *to_free = NULL;

    // include header
    {
        StrArray include = DynArray_INIT;
        push_string(&include, "#include \"");
        push_string(&include, file_name);
        push_string(&include, ".h\"");
        StrArray_push(&include, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@include_asset_h@", include.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&include);
    }

    // file init
    {
        StrArray fi = DynArray_INIT;
        char size_str[12]; // max length for an 32 bit int
        sprintf(size_str, "%zu", files_size);
        push_string(&fi, "#define FILES_SIZE ");
        push_string(&fi, size_str);
        push_string(&fi, "\n");

        for (int i = 0; i < files_size; i++) {
            char *init = generate_file_init_on_heap(i, files[i].name, files[i].data, files[i].size);
            push_string(&fi, init);
            free(init);
        }
        StrArray_push(&fi, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@file_init@", fi.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&fi);
    }

    // list init
    {
        StrArray li = DynArray_INIT;
        for (int i = 0; i < files_size; i++) {
            char *init = generate_list_init_on_heap(i);
            push_string(&li, init);
            free(init);
        }
        StrArray_push(&li, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@list_init@", li.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&li);
    }

    // map init
    {
        StrArray mi = DynArray_INIT;
        for (int i = 0; i < files_size; i++) {
            char *init = generate_map_init_on_heap(i);
            push_string(&mi, init);
            free(init);
        }
        StrArray_push(&mi, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@map_init@", mi.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&mi);
    }


    // asset with namespace
    {
        StrArray name = DynArray_INIT;
        if (namespace) {
            push_string(&name, namespace);
            StrArray_push(&name, '_');
        }
        push_string(&name, "asset");
        StrArray_push(&name, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@asset@", name.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        StrArray_kill(&name);
    }

    return res;
}
