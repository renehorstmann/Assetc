#include <ctype.h>
#include <stdio.h>

#include "utilc/dynarray.h"
#include "utilc/strviu.h"
#include "generate.h"
#include "apply.h"

DynArray(char, StrArray, str_array)


static void push_string(StrArray *array, const char *str) {
    str_array_push_array(array, str, strlen(str));
}

static void push_string_as_upper(StrArray *array, const char *str) {
    while (*str)
        str_array_push(array, (char) toupper(*str++));
}


char *apply_header(const char *template, const char *file_name, const char *namespace) {
    strviu tmp = ToStrViu(template);
    char *res;
    char *to_free = NULL;

    // include guard
    {
        StrArray ig = {0};
        push_string(&ig, "#ifndef ASSETC_");
        if (namespace) {
            push_string_as_upper(&ig, namespace);
            str_array_push(&ig, '_');
        }
        push_string_as_upper(&ig, file_name);
        push_string(&ig, "_H\n#define ASSETC_");
        if (namespace) {
            push_string_as_upper(&ig, namespace);
            str_array_push(&ig, '_');
        }
        push_string_as_upper(&ig, file_name);
        push_string(&ig, "_H\n");
        str_array_push(&ig, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@include_quard@", ig.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&ig);
    }

    // asset with namespace
    {
        StrArray name = {0};
        if (namespace) {
            push_string(&name, namespace);
            str_array_push(&name, '_');
        }
        push_string(&name, "asset");
        str_array_push(&name, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@asset@", name.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&name);
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
        StrArray include = {0};
        push_string(&include, "#include \"");
        push_string(&include, file_name);
        push_string(&include, ".h\"");
        str_array_push(&include, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@include_asset_h@", include.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&include);
    }

    // file init
    {
        StrArray fi = {0};
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
        str_array_push(&fi, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@file_init@", fi.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&fi);
    }

    // name list
    {
        StrArray ni = {0};
        for(int i = 0; i < files_size; i++) {
            push_string(&ni, "        \"");
            push_string(&ni, files[i].name);
            push_string(&ni, "\",");
            if(i < files_size-1)
                str_array_push(&ni, '\n');
        }
        str_array_push(&ni, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@name_list@", ni.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&ni);
    }

    // list init
    {
        StrArray li = {0};
        for (int i = 0; i < files_size; i++) {
            char *init = generate_list_init_on_heap(i);
            push_string(&li, init);
            free(init);
        }
        str_array_push(&li, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@list_init@", li.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&li);
    }

    // map init
    {
        StrArray mi = {0};
        for (int i = 0; i < files_size; i++) {
            char *init = generate_map_init_on_heap(i);
            push_string(&mi, init);
            free(init);
        }
        str_array_push(&mi, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@map_init@", mi.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&mi);
    }


    // asset with namespace
    {
        StrArray name = {0};
        if (namespace) {
            push_string(&name, namespace);
            str_array_push(&name, '_');
        }
        push_string(&name, "asset");
        str_array_push(&name, '\0');

        res = sv_replace_to_heap_cstring(tmp, "@asset@", name.array);
        tmp = ToStrViu(res);

        free(to_free);
        to_free = res;
        str_array_kill(&name);
    }

    return res;
}
