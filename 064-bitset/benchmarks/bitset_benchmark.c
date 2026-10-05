#include "int_bitset.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t n=10000000;
    IntBitset *a=int_bitset_create(n);
    IntBitset *b=int_bitset_create(n);
    IntBitset *out=int_bitset_create(n);

    if(a==NULL||b==NULL||out==NULL)return 1;

    for(size_t i=0;i<n;i+=3)int_bitset_set(a,i);
    for(size_t i=0;i<n;i+=5)int_bitset_set(b,i);

    struct timespec x,y;

    timespec_get(&x,TIME_UTC);
    for(int r=0;r<100;++r) {
        if(!int_bitset_and(out,a,b))return 1;
    }
    timespec_get(&y,TIME_UTC);

    printf("bits=%zu count=%zu 100_and_seconds=%.9f\n",
           n,int_bitset_count(out),elapsed(x,y));

    int_bitset_free(out);
    int_bitset_free(b);
    int_bitset_free(a);
    return 0;
}
