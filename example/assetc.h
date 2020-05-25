#ifndef ASSETC_H
#define ASSETC_H

#include <stdlib.h>

/**
 * Simple struct that holds the memory location and size of an compiled asset file.
 * data[size] will be 0, so a text file will be a valid C string.
 */
typedef struct asset {
    const char *data;
    size_t size;
} asset;

/**
 * Returns the memory location and size of the compiled file.
 * If the file was not found, the asset will be (asset) {NULL, 0}.
 */
asset asset_get(const char *file);

#endif //ASSETC_H