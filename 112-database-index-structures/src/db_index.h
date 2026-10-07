#ifndef DB_INDEX_H
#define DB_INDEX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint64_t primary_key;
    uint64_t secondary_key;
    int64_t payload;
} DbRow;

typedef struct DbIndex DbIndex;

DbIndex *dbi_build(const DbRow *rows,size_t count);
void dbi_free(DbIndex *index);
size_t dbi_count(const DbIndex *index);

bool dbi_get_primary(const DbIndex *index,uint64_t primary_key,
                     DbRow *out_row,bool *out_found);

bool dbi_secondary_equal(const DbIndex *index,uint64_t secondary_key,
                         DbRow *out,size_t out_capacity,size_t *out_count);

bool dbi_secondary_range(const DbIndex *index,uint64_t low,uint64_t high,
                         DbRow *out,size_t out_capacity,size_t *out_count);

bool dbi_validate(const DbIndex *index);

#endif
