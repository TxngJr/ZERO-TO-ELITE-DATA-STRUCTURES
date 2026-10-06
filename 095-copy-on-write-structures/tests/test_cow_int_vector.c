#include "cow_int_vector.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
enum{CLONES=256,BASE=1024};
static uint64_t st=UINT64_C(0x9e3779b97f4a7c15);
static uint64_t rng(void){st^=st<<7;st^=st>>9;st^=st<<8;return st;}
int main(void){int*base=malloc(BASE*sizeof*base);assert(base);for(size_t i=0;i<BASE;++i)base[i]=(int)i;CowIntVector*origin=cow_vector_from_array(base,BASE);assert(origin&&cow_vector_validate(origin));CowIntVector*clones[CLONES];for(size_t i=0;i<CLONES;++i){clones[i]=cow_vector_clone(origin);assert(clones[i]);}assert(cow_vector_share_count(origin)==CLONES+1U);
for(size_t i=0;i<CLONES;++i){size_t idx=(size_t)(rng()%BASE);int old=0;assert(cow_vector_get(clones[i],idx,&old));assert(cow_vector_set(clones[i],idx,old+1000000+(int)i));assert(cow_vector_share_count(clones[i])==1U);int orig=0;assert(cow_vector_get(origin,idx,&orig)&&orig==(int)idx);assert(cow_vector_push(clones[i],(int)i));assert(cow_vector_size(clones[i])==BASE+1U);assert(cow_vector_validate(clones[i]));}
assert(cow_vector_share_count(origin)==1U);CowIntVector*a=cow_vector_clone(origin),*b=cow_vector_clone(origin);assert(a&&b);int x=0;assert(cow_vector_pop(a,&x)&&x==BASE-1);assert(cow_vector_size(a)==BASE-1U&&cow_vector_size(b)==BASE&&cow_vector_size(origin)==BASE);cow_vector_free(a);cow_vector_free(b);for(size_t i=0;i<CLONES;++i)cow_vector_free(clones[i]);cow_vector_free(origin);free(base);
CowIntVector*empty=cow_vector_create();assert(empty&&cow_vector_validate(empty));assert(!cow_vector_pop(empty,NULL));for(int i=0;i<10000;++i)assert(cow_vector_push(empty,i));assert(cow_vector_size(empty)==10000U);cow_vector_free(empty);puts("Copy-on-write vector tests passed");return 0;}
