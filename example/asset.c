// warning, this file was autogenerated/ compiled with assetc.
// See https://github.com/renehorstmann/Assetc

#include <string.h>
#include <stdbool.h>

#include "asset.h"


//
// AUTOGENERATED FILES
//
static const char name_0[] = "hello.txt";
static const char data_0[] =
        "Hello World";

static const char name_1[] = "bin/array.bin";
static const char data_1[] =
        "\x00\xff\x0f\xf0";

//
// END OF AUTOGENERATED FILES
//

static unsigned hash(const char *key) {
    unsigned hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */
    return hash;
}

// linked list of asset
typedef struct llist {
    const char *key;
    asset value;
    struct llist *next;
} llist;

#define LLIST_INIT(name, data) \
{ \
    .key = name, \
    .value = (asset) {data, sizeof(data) -1},\
    .next = NULL \
}


// hash map -> array of linked list of asset (so if to items got the same hash, they end up in the list)
#define MAP_SIZE 2
static llist *map[MAP_SIZE];

static void map_add(llist *list_item_ptr, const char *name) {
    llist **map_item = &map[hash(name) % MAP_SIZE];
    while (*map_item)
        map_item = &((*map_item)->next);
    *map_item = list_item_ptr;
}

static llist *map_get(const char *name) {
    llist *item = map[hash(name) % MAP_SIZE];
    while (item && strcmp(item->key, name) != 0)
        item = item->next;
    return item;
}

static void map_init() {
    // set all map entries to NULL
    memset(map, 0, sizeof(map));

    //
    // AUTOGENERATED MAP INIT
    //

    // list all linked list items
    static llist list_0 = LLIST_INIT(name_0, data_0);
    static llist list_1 = LLIST_INIT(name_1, data_1);

    // add all lists into the map
    map_add(&list_0, name_0);
    map_add(&list_1, name_1);

    //
    // END OF AUTOGENERATED MAP INIT
    //
}


asset asset_get(const char *file) {
    static bool loaded = false;
    if (!loaded) {
        loaded = true;
        map_init();
    }

    asset res = {NULL, 0};
    llist *item = map_get(file);
    if (item)
        res = item->value;
    return res;
}
