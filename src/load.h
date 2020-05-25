#ifndef ASSETC_LOAD_H
#define ASSETC_LOAD_H

#include "file.h"

void load_files(File **out_files, size_t *out_files_size, const char **file_names, size_t file_names_size);

#endif //ASSETC_LOAD_H
