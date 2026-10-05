#ifndef INT_BITMAP_H
#define INT_BITMAP_H

#include <stdbool.h>
#include <stddef.h>

typedef struct IntBitmap IntBitmap;

IntBitmap *int_bitmap_create(size_t universe_size);
void int_bitmap_free(IntBitmap *bitmap);

size_t int_bitmap_universe_size(const IntBitmap *bitmap);
size_t int_bitmap_cardinality(const IntBitmap *bitmap);

bool int_bitmap_contains(const IntBitmap *bitmap,size_t value,bool *out_present);
bool int_bitmap_add(IntBitmap *bitmap,size_t value);
bool int_bitmap_remove(IntBitmap *bitmap,size_t value);

bool int_bitmap_set_range(IntBitmap *bitmap,size_t begin,size_t end);
bool int_bitmap_clear_range(IntBitmap *bitmap,size_t begin,size_t end);

bool int_bitmap_union(IntBitmap *out,const IntBitmap *a,const IntBitmap *b);
bool int_bitmap_intersection(IntBitmap *out,const IntBitmap *a,const IntBitmap *b);
bool int_bitmap_difference(IntBitmap *out,const IntBitmap *a,const IntBitmap *b);

bool int_bitmap_next_member(
    const IntBitmap *bitmap,
    size_t start,
    size_t *out_value
);

bool int_bitmap_validate(const IntBitmap *bitmap);

#endif
