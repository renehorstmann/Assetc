#include <stdio.h>
#include <assert.h>
#include "asset.h"


/*
 * The compiled files are:
 * assets/hello.txt -> ["Hello World"]
 * assets/bin/array.bin -> ["\x00\xff\x0f\xf0"]
 * compiled with $ assetc assets
 */

int main() {

    // returns the memory location and size of a compiled asset file.
    asset hello = asset_get("hello.txt");

    // if asset_get fails, it will raise SIGABRT

    // all files end up with a 0 in memory, so that they are valid C strings
    assert(hello.data[hello.size] == 0);
    printf("hello size: %zu\n", hello.size);
    printf("hello data: %p\n", (const void *) hello.data);
    printf("hello content: <%s>\n\n", hello.data);

    asset array = asset_tryget("bin/array.bin");

    // if asset_tryget fails, .data and .size will be 0

    if(!array.data) {
        fprintf(stderr, "asset array.bin not found");
        return 1;
    }
    assert(array.data[array.size] == 0);
    printf("array size: %zu\n", array.size);
    printf("array data: %p\n", (const void *) array.data);
    printf("array content: <");
    for(int i=0; i<array.size; i++)
        printf(" 0x%02hhx ", array.data[i]);
    puts(">");


    asset not_available = asset_tryget("error");
    assert(!not_available.data);

    // asset_get("error");  // would raise SIGABRT

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
