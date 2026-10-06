#include "object_pool.h"
#include <stdio.h>

typedef struct { int id; double value; } Item;

int main(void) {
    ObjectPool *pool = op_create(sizeof(Item), 8);
    if (!pool) return 1;

    Item *item = op_alloc(pool);
    if (!item) return 2;
    item->id = 42;
    item->value = 3.5;

    printf("id=%d value=%.1f live=%zu stride=%zu\n",
           item->id, item->value,
           op_in_use_count(pool), op_stride(pool));

    if (!op_release(pool, item)) return 3;
    op_free(pool);
    return 0;
}
