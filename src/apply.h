#ifndef ASSETC_APPLY_H
#define ASSETC_APPLY_H

#include "file.h"

char *apply_header(const char *template, const char *file_name, const char *namespace);

char *apply_source(const char *template, const char *file_name, const char *namespace,
                   const File *files, size_t files_size);

#endif //ASSETC_APPLY_H
