#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utilc/alloc.h"
#include "generate.h"
#include "file.h"
#include "load.h"
#include "apply.h"
#include "asset.h"

static void print_help(const char *name) {
    fprintf(stderr, "Usage: %s dir [-h] [-o name]\n"
                    "       -o: sets the out name for the\n"
                    "           asset source and header file\n"
                    "           (default is asset.c|.h)\n"
                    "       -h: will clone asset.h into the\n"
                    "           current dir (with the name from -o)\n",
                    name);
    exit(EXIT_FAILURE);
}

struct args {
    bool clone_header;
    const char *out_name;
    const char *dir;
};

static struct args args_parse(int argc, char **argv) {
    struct args res = {false, "asset", NULL};

    for(int i=1; i<argc; i++) {
        if(argv[i][0] != '-') {
            if(res.dir)
                print_help(argv[0]);
            res.dir = argv[i];

        } else {
            if(strcmp(argv[i], "-o") == 0) {
                if(i == argc-1 || argv[i+1][0] == '-')
                    print_help(argv[0]);
                res.out_name = argv[++i];
            } else if(strcmp(argv[i], "-h") == 0) {
                res.clone_header = true;
            } else
                print_help(argv[0]);
        }
    }

    if(!res.dir)
        print_help(argv[0]);

    return res;
}

int main(int argc, char **argv) {
    struct args args = args_parse(argc, argv);

    File *files;
    size_t files_size = 0;
    {
        char *dir = New(char, strlen(args.dir) + 2);
        strcpy(dir, args.dir);
        if(dir[strlen(dir)] != '/')
            strcat(dir, "/");
 
        load_dir(&files, &files_size, dir);
        free(dir);
    }

    if(args.clone_header) {
        asset template = asset_get("template_header.txt");
        assert(template.data);
        char *header = apply_header(template.data, args.out_name);

        char *name = New(char, strlen(args.out_name) + 3); // + .h\0
        strcpy(name, args.out_name);
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
        char *source = apply_source(template.data, args.out_name, files, files_size);

        char *name = New(char, strlen(args.out_name) + 3); // + .c\0
        strcpy(name, args.out_name);
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
