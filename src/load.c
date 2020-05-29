#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utilc/alloc.h"
#include "utilc/dynarray.h"
#include "load.h"

#define PATH_MAX 4096

DynArray(File, FileArray)

static void error(const char *file_name) {
    fprintf(stderr, "file: <%s> could not be loaded\n", file_name);
    exit(EXIT_FAILURE);
}

static bool is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

static bool is_dir(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void load_dir(File **out_files, size_t *out_files_size, const char *dir_path) {
    FileArray files = DynArray_INIT;

    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    if(!dir)
        return;
    
    while( (entry = readdir(dir)) ) {
        if(entry->d_name[0] == '.')
            continue;

        char *path = New(char, strlen(dir_path) + strlen(entry->d_name) + 2); // + [/] + \0
        strcpy(path, dir_path);
        strcat(path, entry->d_name);

        if(is_dir(path)) {
            strcat(path, "/");
            printf("loading sub dir: %s\n", entry->d_name);
            File *sub_files;
            size_t sub_files_size = 0;

            // todo: file names in sub dir wrong, should include sub folder name...
            load_dir(&sub_files, &sub_files_size, path);
            FileArray_push_array(&files, sub_files, sub_files_size);
            printf("finished sub dir: %s\n", entry->d_name);

        } else if(is_regular_file(path)) {
            printf("loading file: %s\n", entry->d_name);
            File file = File_load(dir_path, entry->d_name);
            FileArray_push(&files, file);
            if(!file.data)
                error(entry->d_name);
        }

        free(path);

    }
    closedir(dir);

    *out_files = files.array;
    *out_files_size = files.size;
}
