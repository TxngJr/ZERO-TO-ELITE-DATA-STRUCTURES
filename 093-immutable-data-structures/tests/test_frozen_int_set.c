#include "frozen_int_set.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t state=UINT64_C(0xa0761d6478bd642f);
static uint64_t rng64(void){state^=state<<13;state^=state>>7;state^=state<<17;return state;}

int main(void){
    FrozenIntSet *empty=frozen_int_set_create(NULL,0);
    assert(empty&&frozen_int_set_validate(empty)&&frozen_int_set_size(empty)==0);
    assert(!frozen_int_set_contains(empty,123)); frozen_int_set_free(empty);
    assert(frozen_int_set_create(NULL,1)==NULL);

    int edge[]={INT_MIN,INT_MAX,0,-1,1,INT_MIN,INT_MAX};
    FrozenIntSet *e=frozen_int_set_create(edge,sizeof edge/sizeof edge[0]);
    assert(e&&frozen_int_set_validate(e)&&frozen_int_set_size(e)==5);
    for(size_t i=0;i<sizeof edge/sizeof edge[0];++i)assert(frozen_int_set_contains(e,edge[i]));
    frozen_int_set_free(e);

    const size_t n=100000,range=50000;
    int *values=malloc(n*sizeof *values); uint8_t *ref=calloc(range,1); assert(values&&ref);
    size_t unique=0;
    for(size_t i=0;i<n;++i){int v=(int)(rng64()%range);values[i]=v;if(!ref[v]){ref[v]=1;++unique;}}
    FrozenIntSet *set=frozen_int_set_create(values,n);
    assert(set&&frozen_int_set_validate(set)&&frozen_int_set_size(set)==unique);
    assert(frozen_int_set_load_factor(set)<=0.5);
    for(size_t k=0;k<range;++k)assert(frozen_int_set_contains(set,(int)k)==(ref[k]!=0));
    for(size_t q=0;q<20000;++q){int k=(int)range+(int)(rng64()%range);assert(!frozen_int_set_contains(set,k));}
    assert(frozen_int_set_storage_bytes(set)>=frozen_int_set_capacity(set)*(sizeof(int)+1U));
    printf("Frozen immutable set tests passed; unique=%zu capacity=%zu\n",unique,frozen_int_set_capacity(set));
    frozen_int_set_free(set); free(values); free(ref); return 0;
}
