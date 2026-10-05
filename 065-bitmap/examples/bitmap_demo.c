#include "int_bitmap.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntBitmap *bitmap=int_bitmap_create(130);
    assert(bitmap!=NULL);

    assert(int_bitmap_add(bitmap,1));
    assert(int_bitmap_add(bitmap,64));
    assert(int_bitmap_add(bitmap,129));
    assert(int_bitmap_set_range(bitmap,10,20));

    size_t first=0;
    assert(int_bitmap_next_member(bitmap,0,&first));

    printf("cardinality=%zu first=%zu\n",
           int_bitmap_cardinality(bitmap),first);

    assert(int_bitmap_validate(bitmap));
    int_bitmap_free(bitmap);
    return 0;
}
