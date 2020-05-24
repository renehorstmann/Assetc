# Assetc
Compile asset files or whole directories into a C source file


## In active development!

## Compilation of assets
To compile an asset file or directory, see the examples below:
```bash
$ assetc example.txt
# compiles the content of example.txt into asset.c

$ assetc example.txt -o asset_file_name.c
# -o changes the name of the compiled file

$ assetc assets_dir
# will recursivly compile all files, except of hidden ones, into asset.c
```


## Usage of compiled assets
Compile the generated asset.c file along your C code and use it like so:
```c
#include <stdio.h>
#include "asset.h"

int main() {
    // get the asset example.txt
    asset example = asset_get("example.txt");
    
    printf("example has a length of %d bytes and the location %p in memory\n", 
           example.size, example.data);

    assert(example.data[example.size] == 0);
    printf("content: <%s>\n", example.data);
}
```
