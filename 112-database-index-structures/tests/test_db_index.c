#include "db_index.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum { N=50000, QUERIES=20000 };
static uint64_t state=UINT64_C(0x1020304050607080);
static uint64_t rng(void){
    state^=state<<13;
    state^=state>>7;
    state^=state<<17;
    return state;
}

int main(void){
    DbRow *rows=malloc((size_t)N*sizeof(*rows));
    assert(rows);

    for(size_t i=0;i<N;++i){
        rows[i]=(DbRow){
            (uint64_t)i*2U+1U,
            (uint64_t)(i%1000U),
            (int64_t)i*17-3
        };
    }

    for(size_t i=N-1U;i>0U;--i){
        size_t j=(size_t)(rng()%(i+1U));
        DbRow t=rows[i];rows[i]=rows[j];rows[j]=t;
    }

    DbIndex *index=dbi_build(rows,N);
    assert(index&&dbi_count(index)==N&&dbi_validate(index));

    for(size_t q=0;q<QUERIES;++q){
        uint64_t key=rng()%((uint64_t)N*2U+100U);
        DbRow got={0};
        bool found=false;
        assert(dbi_get_primary(index,key,&got,&found));
        bool expected=key<(uint64_t)N*2U&&((key&1U)!=0U);
        assert(found==expected);
        if(found){
            size_t i=(size_t)((key-1U)/2U);
            assert(got.primary_key==key);
            assert(got.secondary_key==i%1000U);
            assert(got.payload==(int64_t)i*17-3);
        }
    }

    DbRow equal[64];
    size_t equal_count=0U;
    assert(dbi_secondary_equal(index,42U,equal,64U,&equal_count));
    assert(equal_count==50U);
    for(size_t i=0;i<equal_count;++i){
        assert(equal[i].secondary_key==42U);
        if(i>0U)assert(equal[i-1U].primary_key<equal[i].primary_key);
    }

    DbRow range[512];
    size_t range_count=0U;
    assert(dbi_secondary_range(index,100U,109U,range,512U,&range_count));
    assert(range_count==500U);
    for(size_t i=0;i<range_count;++i){
        assert(range[i].secondary_key>=100U&&range[i].secondary_key<=109U);
        if(i>0U){
            assert(range[i-1U].secondary_key<range[i].secondary_key||
                   (range[i-1U].secondary_key==range[i].secondary_key&&
                    range[i-1U].primary_key<range[i].primary_key));
        }
    }

    DbRow dup[2]={{1U,1U,1},{1U,2U,2}};
    assert(dbi_build(dup,2U)==NULL);

    puts("Database index tests passed");
    dbi_free(index);
    free(rows);
    return 0;
}
