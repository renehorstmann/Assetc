#ifndef ASSETC_ASSET_H
#define ASSETC_ASSET_H


#include <stdlib.h> // for size_t

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
 * If the file was not found, SIGABRT will be raised
 */
asset asset_get(const char *file);


/**
 * Returns the memory location and size of the compiled file.
 * If the file was not found, .data and .size will be 0.
 */
asset asset_tryget(const char *file);

#endif //end of include quard