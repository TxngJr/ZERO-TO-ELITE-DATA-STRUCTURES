#ifndef INT_INT_HASH_TABLE_H
#define INT_INT_HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntIntHashTable IntIntHashTable;

IntIntHashTable *int_int_hash_table_create(void);
void int_int_hash_table_free(IntIntHashTable *table);

size_t int_int_hash_table_size(const IntIntHashTable *table);
size_t int_int_hash_table_bucket_count(const IntIntHashTable *table);
double int_int_hash_table_load_factor(const IntIntHashTable *table);

bool int_int_hash_table_put(IntIntHashTable *table, int key, int value);
bool int_int_hash_table_get(const IntIntHashTable *table, int key, int *out_value);
bool int_int_hash_table_contains(const IntIntHashTable *table, int key);
bool int_int_hash_table_remove(IntIntHashTable *table, int key, int *out_value);
bool int_int_hash_table_validate(const IntIntHashTable *table);

#endif
