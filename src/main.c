#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utilc/alloc.h"
#include "generate.h"
#include "file.h"
#include "load.h"
#include "apply.h"

#define USE_TEMPLATES_FROM_ASSET

#ifdef USE_TEMPLATES_FROM_ASSET
#include "asset.h"
#endif

static const char *program_name;
static void print_help() {
    fprintf(stderr, "Usage: %s dir [-h] [-o name]\n"
                    "       -h: will clone asset.h into the\n"
                    "           current dir (with the name from -o)\n"
                    "       -o: sets the file name for the\n"
                    "           asset source and header file\n"
                    "           (default is asset.c|.h)\n"
                    "       -n: sets the namespace for globals\n",
                    program_name);
    exit(EXIT_FAILURE);
}


static char *open_file_as_string(const char *filename) {
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
    return text;
}

static const char *get_template_header() {
#ifdef USE_TEMPLATES_FROM_ASSET
    return asset_get("template_header.txt").data;
#else
    return open_file_as_string("../assets/template_header.txt");
#endif
}

static const char *get_template_source() {
#ifdef USE_TEMPLATES_FROM_ASSET
    return asset_get("template_source.txt").data;
#else
    return open_file_as_string("../assets/template_source.txt");
#endif
}

struct args {
    bool clone_header;
    const char *file_name;
    const char *namespace;
    const char *dir;
};

static struct args args_parse(int argc, char **argv) {
    struct args res = {false, "asset", NULL, NULL};

    for(int i=1; i<argc; i++) {
        if(argv[i][0] != '-') {
            if(res.dir)
                print_help();
            res.dir = argv[i];

        } else {
            if(strcmp(argv[i], "-h") == 0) {
                res.clone_header = true;
            } else if(strcmp(argv[i], "-o") == 0) {
                if(i == argc-1 || argv[i+1][0] == '-')
                    print_help();
                res.file_name = argv[++i];
            } else if(strcmp(argv[i], "-n") == 0) {
                res.namespace = argv[++i];
            } else
                print_help();
        }
    }

    if(!res.dir)
        print_help();

    // check strlen(res.out_name) < 40

    return res;
}

int main(int argc, char **argv) {
    program_name = argv[0];
    struct args args = args_parse(argc, argv);

    File *files = NULL;
    size_t files_size = 0;

    {
        char *dir = New(char, strlen(args.dir) + 2);  // + [/] + \0
        strcpy(dir, args.dir);
        if (dir[strlen(dir) - 1] != '/')
            strcat(dir, "/");

        load_dir(&files, &files_size, dir);
        if(!files)
            print_help();
        free(dir);
    }

    if(args.clone_header) {
        char *header = apply_header(get_template_header(), args.file_name, args.namespace);

        char *name = New(char, strlen(args.file_name) + 3); // + .h\0
        strcpy(name, args.file_name);
        strcat(name, ".h");

        FILE *file = fopen(name, "w");
        fprintf(file, "%s", header);

        fclose(file);
        free(name);
        free(header);
    }

    {
        char *source = apply_source(get_template_source(), args.file_name, args.namespace, files, files_size);

        char *name = New(char, strlen(args.file_name) + 3); // + .c\0
        strcpy(name, args.file_name);
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
