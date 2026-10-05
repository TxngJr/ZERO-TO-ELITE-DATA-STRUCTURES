#include "int_btree.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t ts[]={2,4,16,64};
    const int n=100000;

    puts("t,height,insert_seconds,lookup_seconds");

    for(size_t j=0;j<4;++j) {
        IntBTree *tree=int_btree_create(ts[j]);
        if(tree==NULL)return 1;

        uint32_t rng=123456789u;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);
        for(int i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            if(!int_btree_insert(tree,(int)(rng&0x7fffffffU))) {
                --i;
            }
        }
        timespec_get(&b,TIME_UTC);
        const double insert_s=elapsed(a,b);

        timespec_get(&a,TIME_UTC);
        volatile int hits=0;
        rng=987654321u;
        for(int i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            hits+=int_btree_contains(tree,(int)(rng&0x7fffffffU))?1:0;
        }
        timespec_get(&b,TIME_UTC);
        const double lookup_s=elapsed(a,b);

        size_t height=0;
        int_btree_height(tree,&height);

        printf("%zu,%zu,%.9f,%.9f\n",
               ts[j],height,insert_s,lookup_s);

        (void)hits;
        int_btree_free(tree);
    }

    puts("Higher fanout lowers height but increases in-node scan work in this linear-scan implementation.");
    return 0;
}
