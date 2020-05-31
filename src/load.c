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
    
    char *current_dir = New(char, strlen(root_dir) + strlen(sub_dir) + 1);
    strcpy(current_dir, root_dir);
    strcat(current_dir, sub_dir);
    
    DIR *dir = opendir(current_dir);
    struct dirent *entry;
    if(!dir) {
        fprintf(stderr, "failed to load dir: %s\n", current_dir);
        exit(EXIT_FAILURE);
    }
    
    while( (entry = readdir(dir)) ) {
        if(entry->d_name[0] == '.')
            continue;

        char *path = New(char, strlen(current_dir) + strlen(entry->d_name) + 2); // + [/] + \0
        strcpy(path, current_dir);
        strcat(path, entry->d_name);

        if(is_dir(path)) {
            strcat(path, "/");
            printf("loading sub dir: %s\n", entry->d_name);
            File *sub_files;
            size_t sub_files_size = 0;

            char *new_sub_dir = New(char, strlen(sub_dir) + strlen(entry->d_name) + 2);
            strcpy(new_sub_dir, sub_dir);
            strcat(new_sub_dir, entry->d_name);
            strcat(new_sub_dir, "/");

            load_dir_r(&sub_files, &sub_files_size, root_dir, new_sub_dir);
            FileArray_push_array(&files, sub_files, sub_files_size);
            printf("finished sub dir: %s\n", entry->d_name);

            free(new_sub_dir);

        } else if(is_regular_file(path)) {
            printf("loading file: %s\n", entry->d_name);
            char *name = New(char, strlen(sub_dir) + strlen(entry->d_name) + 1);
            strcpy(name, sub_dir);
            strcat(name , entry->d_name);
            File file = File_load(root_dir, name);
            FileArray_push(&files, file);
            if(!file.data) {
                fprintf(stderr, "failed to load file: %s\n", name);
                exit(EXIT_FAILURE);
            }

            free(name);
        }

        free(path);

    }
    closedir(dir);
    free(current_dir);

    *out_files = files.array;
    *out_files_size = files.size;
}

void load_dir(File **out_files, size_t *out_files_size, const char *root_dir) {
    load_dir_r(out_files, out_files_size, root_dir, "");
}
