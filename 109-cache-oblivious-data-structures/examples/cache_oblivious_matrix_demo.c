#include "cache_oblivious_matrix.h"
#include <stdio.h>
int main(void){
    CacheObliviousMatrix*m=com_create(5U,7U);
    if(!m)return 1;
    for(size_t r=0;r<5U;++r)for(size_t c=0;c<7U;++c)com_set(m,r,c,(double)(r*10U+c));
    printf("rows=%zu cols=%zu padded=%zu row_sum=%.0f z_sum=%.0f\n",
           com_rows(m),com_cols(m),com_padded_side(m),
           com_sum_row_order(m),com_sum_z_order(m));
    com_free(m);return 0;
}
