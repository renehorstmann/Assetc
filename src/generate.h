#ifndef ASSETC_GENERATE_H
#define ASSETC_GENERATE_H

#include <stdlib.h>

char *generate_file_init_on_heap(int id, const char *name, const char *data, size_t size);

char *generate_list_init_on_heap(int id);

char *generate_map_init_on_heap(int id);

#endif //ASSETC_GENERATE_H
