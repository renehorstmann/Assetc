#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utilc/assume.h"
#include "utilc/alloc.h"
#include "utilc/dynarray.h"
#include "utilc/strpool.h"
#include "load.h"

#define PATH_MAX 4096

DynArray(File, FileArray, file_array)

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

static void load_dir_r(File **out_files, size_t *out_files_size, const char *root_dir, const char *sub_dir) {
    FileArray files = DynArray_INIT;

    char *current_dir = sp_cat(root_dir, sub_dir);
    
    DIR *dir = opendir(current_dir);
    struct dirent *entry;
    assume(dir, "failed to load dir: %s", current_dir);
    
    while( (entry = readdir(dir)) ) {
        if(entry->d_name[0] == '.')
            continue;

        char *path = sp_cat_n(1, current_dir, entry->d_name);

        if(is_dir(path)) {
            strcat(path, "/");
            printf("loading sub dir: %s\n", entry->d_name);
            File *sub_files;
            size_t sub_files_size = 0;
            char *new_sub_dir = sp_cat(sub_dir, entry->d_name, "/");
            load_dir_r(&sub_files, &sub_files_size, root_dir, new_sub_dir);
            file_array_push_array(&files, sub_files, sub_files_size);
            printf("finished sub dir: %s\n", entry->d_name);

        } else if(is_regular_file(path)) {
            printf("loading file: %s\n", entry->d_name);
            char *name = sp_cat(sub_dir, entry->d_name);
            File file = File_load(root_dir, name);
            file_array_push(&files, file);
            assume(file.data, "failed to load file: %s", name);
        }
    }
    closedir(dir);
    sp_free();

    *out_files = files.array;
    *out_files_size = files.size;
}

void load_dir(File **out_files, size_t *out_files_size, const char *root_dir) {
    load_dir_r(out_files, out_files_size, root_dir, "");
}
