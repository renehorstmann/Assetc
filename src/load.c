#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "utilc/dynarray.h"
#include "load.h"

#define PATH_MAX 4096

DynArray(File, FileArray)

static void error(FileArray *files, const char *file_name) {
    for(int i=0; i<files->size; i++)
        File_kill(&files->array[i]);
    FileArray_kill(files);
    fprintf(stderr, "file: <%s> could not be loaded\n", file_name);
    exit(EXIT_FAILURE);
}

static int is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

static int is_dir(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void load_dir(File **out_files, size_t *out_files_size, const char *dir_path) {
    FileArray files = DynArray_INIT;

    char path[PATH_MAX];
    strcpy(path, dir_path);
    char *path_append = path + strlen(dir_path);
    if(path_append[-1] != '/')
        *path_append++ = '/';
 
        

    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    if(!dir)
        return;
    
    while( (entry = readdir(dir)) ) {
        strcpy(path_append, entry->d_name);
        puts(path);
    }
    // todo

    closedir(dir);

//    for(int i=0; i<file_names_size; i++) {
//        File file = File_load(file_names[i]);
//        FileArray_push(&files, file);
//        if(!file.data)
//            error(&files, file_names[i]);
//    }
//
//    *out_files = files.array;
//    *out_files_size = files.size;
}
