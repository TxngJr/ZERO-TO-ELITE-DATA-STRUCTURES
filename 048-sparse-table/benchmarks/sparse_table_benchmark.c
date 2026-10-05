#include "int_sparse_table.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double elapsed(struct timespec a,struct timespec b) {
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1000000000.0;
}

int main(void) {
    const size_t sizes[]={1024,16384,262144};
    const size_t queries=500000;

    uint32_t rng=123456789u;

    puts("n,queries,query_seconds");

    for(size_t s=0;s<3;++s) {
        const size_t n=sizes[s];

        int64_t *values=malloc(n*sizeof *values);
        if(values==NULL)return 1;

        for(size_t i=0;i<n;++i) {
            rng=rng*1664525u+1013904223u;
            values[i]=(int64_t)(rng%100000U);
        }

        IntSparseTable *table=
            int_sparse_table_create(values,n);
        if(table==NULL)return 1;

        volatile int64_t checksum=0;
        struct timespec a,b;

        timespec_get(&a,TIME_UTC);

        for(size_t q=0;q<queries;++q) {
            rng=rng*1664525u+1013904223u;
            size_t left=rng%n;

            rng=rng*1664525u+1013904223u;
            size_t right=rng%n;

            if(left>right) {
                const size_t tmp=left;
                left=right;
                right=tmp;
            }

            ++right;

            int64_t value=0;

            if(!int_sparse_table_range_min(
                    table,left,right,&value
                )) {
                return 1;
            }

            checksum^=value;
        }

        timespec_get(&b,TIME_UTC);

        printf("%zu,%zu,%.9f\n",
               n,queries,elapsed(a,b));

        (void)checksum;
        int_sparse_table_free(table);
        free(values);
    }

    puts("Query timing excludes Theta(n log n) preprocessing; Sparse Table is intended for static query-heavy workloads.");
    return 0;
}
