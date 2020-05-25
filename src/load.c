#include <stdio.h>
#include "utilc/dynarray.h"
#include "load.h"

DynArray(File, FileArray)

void error(FileArray *files, const char *file_name) {
    for(int i=0; i<files->size; i++)
        File_kill(&files->array[i]);
    FileArray_kill(files);
    fprintf(stderr, "file: <%s> could not be loaded\n", file_name);
    exit(EXIT_FAILURE);
}

void load_files(File **out_files, size_t *out_files_size, const char **file_names, size_t file_names_size) {
    FileArray files = DynArray_INIT;

    for(int i=0; i<file_names_size; i++) {
        File file = File_load(file_names[i]);
        FileArray_push(&files, file);
        if(!file.data)
            error(&files, file_names[i]);
    }

    *out_files = files.array;
    *out_files_size = files.size;
}
