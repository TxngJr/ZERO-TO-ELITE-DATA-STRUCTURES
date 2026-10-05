#include "int_int_hash_table.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntIntHashTable *table = int_int_hash_table_create();
    assert(table != NULL);

    assert(int_int_hash_table_put(table, 10, 100));
    assert(int_int_hash_table_put(table, 20, 200));
    assert(int_int_hash_table_put(table, 10, 999));

    int value = 0;
    assert(int_int_hash_table_get(table, 10, &value));
    printf("key=10 value=%d size=%zu buckets=%zu alpha=%.3f\n",
           value,
           int_int_hash_table_size(table),
           int_int_hash_table_bucket_count(table),
           int_int_hash_table_load_factor(table));

    assert(int_int_hash_table_remove(table, 20, &value));
    printf("removed key=20 value=%d\n", value);

    assert(int_int_hash_table_validate(table));
    int_int_hash_table_free(table);
    return 0;
}
