#include <stdio.h>
#include <assert.h>
#include "assetc.h"


/*
 * The compiled files are:
 * hello.txt -> ["Hello World"]
 * bin/array.bin -> ["\x00\xff\x0f\xf0"]
 */

int main() {

    // returns the memory location and size of a compiled asset file.
    asset hello = asset_get("hello.txt");

    // if .data or .size == 0, the file was not found
    if(!hello.data) {
        fprintf(stderr, "asset hello.txt not found");
        return EXIT_FAILURE;
    }

    // all files end up with a 0 in memory, so that they are valid C strings
    assert(hello.data[hello.size] == 0);
    printf("hello size: %zu\n", hello.size);
    printf("hello data: %p\n", (const void *) hello.data);
    printf("hello content: <%s>\n\n", hello.data);

    asset array = asset_get("bin/array.bin");
    if(!array.data) {
        fprintf(stderr, "asset array.bin not found");
        return EXIT_FAILURE;
    }
    assert(array.data[array.size] == 0);
    printf("array size: %zu\n", array.size);
    printf("array data: %p\n", (const void *) array.data);
    printf("array content: <");
    for(int i=0; i<array.size; i++)
        printf(" 0x%02hhx ", array.data[i]);
    puts(">");


    asset not_available = asset_get("error");
    assert(!not_available.data);

    // no need to free up asset

    // will print:
    //hello size: 11
    //hello data: 0x55ed90a58000
    //hello content: <Hello World>
    //
    //array size: 4
    //array data: 0x55ed90a5801a
    //array content: < 0x00  0xff  0x0f  0xf0 >
}