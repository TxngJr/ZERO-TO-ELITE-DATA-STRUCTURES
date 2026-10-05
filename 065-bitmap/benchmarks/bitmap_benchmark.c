#include "int_bitmap.h"

#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t universe=10000000;
    IntBitmap *a=int_bitmap_create(universe);
    IntBitmap *b=int_bitmap_create(universe);
    IntBitmap *out=int_bitmap_create(universe);

    if(a==NULL||b==NULL||out==NULL)return 1;

    for(size_t i=0;i<universe;i+=3)int_bitmap_add(a,i);
    for(size_t i=0;i<universe;i+=5)int_bitmap_add(b,i);

    struct timespec x,y;

    timespec_get(&x,TIME_UTC);

    for(int r=0;r<100;++r) {
        if(!int_bitmap_intersection(out,a,b))return 1;
    }

    timespec_get(&y,TIME_UTC);

    printf(
        "universe=%zu cardinality=%zu 100_intersections=%.9f\n",
        universe,
        int_bitmap_cardinality(out),
        elapsed(x,y)
    );

    int_bitmap_free(out);
    int_bitmap_free(b);
    int_bitmap_free(a);
    return 0;
}
