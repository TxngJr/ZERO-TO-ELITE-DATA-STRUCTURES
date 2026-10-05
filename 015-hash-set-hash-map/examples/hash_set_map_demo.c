#include "int_hash_set.h"
#include "string_int_hash_map.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntHashSet *set = int_hash_set_create();
    StringIntHashMap *map = string_int_hash_map_create();
    assert(set != NULL && map != NULL);

    assert(int_hash_set_add(set, 10));
    assert(int_hash_set_add(set, 20));
    assert(int_hash_set_add(set, 10));
    printf("set size=%zu contains20=%d\n",
           int_hash_set_size(set),
           int_hash_set_contains(set, 20));

    char key[] = "alice";
    assert(string_int_hash_map_put(map, key, 100));
    key[0] = 'X';

    int value = 0;
    assert(string_int_hash_map_get(map, "alice", &value));
    printf("map alice=%d size=%zu capacity=%zu\n",
           value,
           string_int_hash_map_size(map),
           string_int_hash_map_capacity(map));

    int_hash_set_free(set);
    string_int_hash_map_free(map);
    return 0;
}
