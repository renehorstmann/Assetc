#include <stdio.h>
#include "generate.h"
#include "load.h"

int main() {

    char *file = generate_file_init_on_heap(550, "example.txt", "Hello World", 11);
    char *list = generate_list_init_on_heap(1234);
    char *map = generate_map_init_on_heap(56880);

    File f = File_load("../README.md");

    File_kill(&f);
    return 0;
}
