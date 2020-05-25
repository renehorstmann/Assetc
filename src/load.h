#ifndef ASSETC_LOAD_H
#define ASSETC_LOAD_H

#include "stdlib.h"

typedef struct File {
    const char *name;
    const char *data;
    size_t size;
} File;

void File_kill(File *self);

File File_load(const char *file_name);

#endif //ASSETC_LOAD_H
