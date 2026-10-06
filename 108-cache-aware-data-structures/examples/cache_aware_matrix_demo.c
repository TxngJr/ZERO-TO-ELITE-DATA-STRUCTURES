#include "cache_aware_matrix.h"
#include <stdio.h>

int main(void){
    CacheAwareMatrix*m=cam_create(5U,7U,2U,4U);
    if(!m)return 1;
    for(size_t r=0;r<5U;++r)
        for(size_t c=0;c<7U;++c)
            cam_set(m,r,c,(double)(r*10U+c));

    printf("row-sum=%.0f tile-sum=%.0f tile=%zux%zu\n",
           cam_sum_row_order(m),cam_sum_tile_order(m),
           cam_tile_rows(m),cam_tile_cols(m));
    cam_free(m);
    return 0;
}
