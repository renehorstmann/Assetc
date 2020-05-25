#include <stdio.h>
#include <string.h>
#include "utilc/alloc.h"
#include "load.h"

void File_kill(File *self) {
    Free0(self->name);
    Free0(self->data);
    self->size = 0;
}

static void open_file_as_string(char **out_string, size_t *out_size, const char *filename) {
    char *text = NULL;
    FILE *file = fopen(filename, "rb");
    long length = 0;
    if (file) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        text = malloc(length);
        if (text)
            fread(text, 1, length, file);
        fclose(file);
    }
    *out_string = text;
    *out_size = length;
}

File File_load(const char *file_name) {
    char *string;
    size_t size;
    open_file_as_string(&string, &size, file_name);
    if(size == 0)
        return (File) {0};
    char *name_copy = New(char, strlen(file_name)+1);
    strcpy(name_copy, file_name);
    return (File) {name_copy, string, size};
}
