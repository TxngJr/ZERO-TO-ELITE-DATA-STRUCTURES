#include "int_sparse_table.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const int64_t values[]={8,3,6,1,7,2,5,4};

    IntSparseTable *table=
        int_sparse_table_create(values,8);
    assert(table!=NULL);

    int64_t min=0;

    assert(int_sparse_table_range_min(
        table,2,7,&min
    ));

    printf("min[2,7)=%" PRId64 " levels=%zu\n",
           min,
           int_sparse_table_levels(table));

    assert(int_sparse_table_validate(table));
    int_sparse_table_free(table);
    return 0;
}
