#ifndef ASSETC_FILE_H
#define ASSETC_FILE_H

#include "stdlib.h"

typedef struct File {
    const char *name;
    const char *data;
    size_t size;
} File;

void File_kill(File *self);

File File_load(const char *dir, const char *file_name);

#endif //ASSETC_FILE_H
