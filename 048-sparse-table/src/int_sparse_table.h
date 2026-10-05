#ifndef INT_SPARSE_TABLE_H
#define INT_SPARSE_TABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntSparseTable IntSparseTable;

IntSparseTable *int_sparse_table_create(
    const int64_t *values,
    size_t count
);
void int_sparse_table_free(IntSparseTable *table);

size_t int_sparse_table_size(const IntSparseTable *table);
size_t int_sparse_table_levels(const IntSparseTable *table);

bool int_sparse_table_range_min(
    const IntSparseTable *table,
    size_t left,
    size_t right,
    int64_t *out_min
);

bool int_sparse_table_validate(
    const IntSparseTable *table
);

#endif
