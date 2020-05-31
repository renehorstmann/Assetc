# Assetc
Compile an asset directory into a C source file.

## Compilation of assets
To compile an asset directory, see the examples below:
```bash
$ assetc assets_dir
# compiles recursivly the content of assets_dir into asset.c

$ assetc -h
# will copy asset.h into the current dir (asset.h is always the same)

$ assetc assetc_dir -o asset_file_name
# -o changes the name of the compiled file

$ asstec assetc_dir -n namespace
# -n adds a namespace to globals (namespace_asset*)
```


## Usage of compiled assets
Compile the generated asset.c file along your C code and use it like so:

```c
#include <stdio.h>
#include "asset.h"
int main() {
    asset example = asset_get("example.txt");
    printf("example data size: %zu\n", example.size);
    puts(example.data);
}
```

The asset struct is defined as:
```c
/**
 * Simple struct that holds the memory location and size of an compiled asset file.
 * data[size] will be 0, so a text file will be a valid C string.
 */
typedef struct asset {
    const char *data;
    size_t size;
} asset;
```

Next comes a bigger example, that is still easy:
```c
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
```

## Open asset as FILE
To get a C FILE handle for an compiled asset, use stdio.h's fmemopen:
```c
#include <stdio.h>
#include "asset.h"

int main() {
    asset example = asset_get("example.txt");
    FILE *file = fmemopen(example.data, example.size, "r");
    
    // do smth with the file handle...

    fclose(file);
}
```
