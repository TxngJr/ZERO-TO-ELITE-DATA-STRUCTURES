#include "db_index.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum { N=200000, QUERIES=100000 };

static double elapsed(struct timespec a,struct timespec b){
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    DbRow *rows=malloc((size_t)N*sizeof(*rows));
    if(!rows)return 1;
    for(size_t i=0;i<N;++i)
        rows[i]=(DbRow){(uint64_t)i*3U+1U,(uint64_t)(i%4096U),(int64_t)i};

    DbIndex *index=dbi_build(rows,N);
    if(!index)return 2;

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    size_t hits=0U;
    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=(uint64_t)(q*7919U)%((uint64_t)N*3U);
        DbRow row={0};
        bool found=false;
        if(!dbi_get_primary(index,key,&row,&found))return 3;
        hits+=found?1U:0U;
    }
    timespec_get(&b,TIME_UTC);

    printf("rows=%d queries=%d seconds=%.6f hits=%zu\n",
           N,QUERIES,elapsed(a,b),hits);

    dbi_free(index);
    free(rows);
    return 0;
}
