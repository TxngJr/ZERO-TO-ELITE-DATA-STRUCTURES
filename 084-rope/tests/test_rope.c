#include "byte_rope.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint32_t next_rng(uint32_t*s){*s=*s*1664525u+1013904223u;return*s;}
int main(void){enum{OPS=20000,MAX=12000};ByteRope*r=byte_rope_create(0x84A12345u);assert(r);uint8_t*model=malloc(MAX),*out=malloc(MAX);assert(model&&out);size_t n=0;uint32_t rng=0x84BEEF12u;
for(int op=0;op<OPS;++op){unsigned kind=next_rng(&rng)%3U;if((kind<2&&n<MAX-4)||n==0){size_t pos=n?next_rng(&rng)%(n+1):0;size_t len=1+(next_rng(&rng)%4U);if(n+len>MAX)len=MAX-n;uint8_t tmp[4];for(size_t j=0;j<len;++j)tmp[j]=(uint8_t)('A'+next_rng(&rng)%26U);assert(byte_rope_insert(r,pos,tmp,len));memmove(model+pos+len,model+pos,n-pos);memcpy(model+pos,tmp,len);n+=len;}else{size_t pos=next_rng(&rng)%n;size_t len=1+(next_rng(&rng)%4U);if(len>n-pos)len=n-pos;assert(byte_rope_erase(r,pos,len));memmove(model+pos,model+pos+len,n-pos-len);n-=len;}assert(byte_rope_size(r)==n);assert(byte_rope_validate(r));assert(byte_rope_copy(r,out,MAX));assert(memcmp(model,out,n)==0);if(n){size_t i=next_rng(&rng)%n;uint8_t v=0;assert(byte_rope_get(r,i,&v)&&v==model[i]);}}
free(out);free(model);byte_rope_free(r);puts("Rope tests passed");return 0;}
