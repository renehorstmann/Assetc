#include <stdio.h>
#include "generate.h"

int main() {

    char *file = generate_file_init_on_heap(550, "example.txt", "Hello World", 11);
    char *list = generate_list_init_on_heap(1234);
    char *map = generate_map_init_on_heap(56880);

    return 0;
}
