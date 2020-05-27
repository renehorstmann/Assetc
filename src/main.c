#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utilc/alloc.h"
#include "generate.h"
#include "file.h"
#include "load.h"
#include "apply.h"
#include "asset.h"

void print_help(const char *name) {
    fprintf(stderr, "Usage: %s dir [-h] [-o name]\n"
                    "       -o: sets the out name for the\n"
                    "           asset source and header file\n"
                    "           (default is asset.c|.h)\n"
                    "       -h: will clone asset.h into the\n"
                    "           current dir (with the name from -o)\n",
                    name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    bool clone_header = false;
    const char *out_name = "asset";
    const char *dir = NULL;

    for(int i=1; i<argc; i++) {
        if(argv[i][0] != '-') {
            if(dir)
                print_help(argv[0]);
            dir = argv[i];

        } else {
            if(strcmp(argv[i], "-o") == 0) {
                if(i == argc-1 || argv[i+1][0] == '-')
                    print_help(argv[0]);
                out_name = argv[++i];
            } else if(strcmp(argv[i], "-h") == 0) {
                clone_header = true;
            } else
                print_help(argv[0]);
        }
    }

    if(!dir)
        print_help(argv[0]);

    File *files;
    size_t files_size = 0;
    {
        char *dir_cpy = New(char, strlen(dir) + 2);
        strcpy(dir_cpy, dir);
        if(dir[strlen(dir)] != '/')
            strcat(dir_cpy, "/");
 
        load_dir(&files, &files_size, dir_cpy);
        free(dir_cpy);
    }

    if(clone_header) {
        asset template = asset_get("template_header.txt");
        assert(template.data);
        char *header = apply_header(template.data, out_name);

        char *name = New(char, strlen(out_name) + 3); // + .h\0
        strcpy(name, out_name);
        strcat(name, ".h");

        FILE *file = fopen(name, "w");
        fprintf(file, "%s", header);

        fclose(file);
        free(name);
        free(header);
    }

    {
        asset template = asset_get("template_source.txt");
        assert(template.data);
        char *source = apply_source(template.data, out_name, files, files_size);

        char *name = New(char, strlen(out_name) + 3); // + .c\0
        strcpy(name, out_name);
        strcat(name, ".c");

        FILE *file = fopen(name, "w");
        fprintf(file, "%s", source);

        fclose(file);
        free(name);
        free(source);
    }

    for(int i=0; i<files_size; i++)
        File_kill(&files[i]);
    free(files);
    return 0;
}
