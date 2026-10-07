#include "compiler_structures.h"
#include <stdio.h>
#include <time.h>

enum { N=20000, QUERIES=200000 };

static double elapsed(struct timespec a,struct timespec b){
    return (double)(b.tv_sec-a.tv_sec)+
           (double)(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(void){
    CompilerTables *t=ct_create(N+16U,N+16U,32U);
    if(!t)return 1;

    for(size_t i=0;i<N;++i){
        char name[32];
        snprintf(name,sizeof(name),"id_%zu",i);
        CtSymbolInfo info={0};
        if(!ct_declare(t,name,CT_SYMBOL_VARIABLE,(int)(i%31U),&info))return 2;
    }

    struct timespec a,b;
    timespec_get(&a,TIME_UTC);
    size_t hits=0U;
    for(size_t q=0;q<QUERIES;++q){
        size_t i=(q*7919U)%N;
        char name[32];
        snprintf(name,sizeof(name),"id_%zu",i);
        CtSymbolInfo info={0};
        bool found=false;
        if(!ct_lookup(t,name,&info,&found))return 3;
        hits+=found?1U:0U;
    }
    timespec_get(&b,TIME_UTC);

    printf("symbols=%d queries=%d seconds=%.6f hits=%zu\n",
           N,QUERIES,elapsed(a,b),hits);
    ct_free(t);
    return 0;
}
