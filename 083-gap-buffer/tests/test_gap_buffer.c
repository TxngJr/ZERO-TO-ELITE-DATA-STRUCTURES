#include "gap_buffer.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint32_t next_rng(uint32_t*s){*s=*s*1664525u+1013904223u;return*s;}
int main(void){enum{OPS=20000,MAX=20000};GapBuffer*b=gap_buffer_create(8);assert(b);uint8_t*model=malloc(MAX);uint8_t*out=malloc(MAX);assert(model&&out);size_t n=0;uint32_t rng=0x83A12345u;
for(int op=0;op<OPS;++op){unsigned kind=next_rng(&rng)%3U;if((kind<2&&n<MAX-4)||n==0){size_t pos=n?next_rng(&rng)%(n+1):0;size_t len=1+(next_rng(&rng)%4U);if(n+len>MAX)len=MAX-n;uint8_t tmp[4];for(size_t j=0;j<len;++j)tmp[j]=(uint8_t)('a'+next_rng(&rng)%26U);assert(gap_buffer_insert(b,pos,tmp,len));memmove(model+pos+len,model+pos,n-pos);memcpy(model+pos,tmp,len);n+=len;}else{size_t pos=next_rng(&rng)%n;size_t max=n-pos;size_t len=1+(next_rng(&rng)%4U);if(len>max)len=max;assert(gap_buffer_erase(b,pos,len));memmove(model+pos,model+pos+len,n-pos-len);n-=len;}assert(gap_buffer_size(b)==n);assert(gap_buffer_validate(b));assert(gap_buffer_copy(b,out,MAX));assert(memcmp(model,out,n)==0);if(n){size_t i=next_rng(&rng)%n;uint8_t v=0;assert(gap_buffer_get(b,i,&v)&&v==model[i]);}}
free(out);free(model);gap_buffer_free(b);puts("Gap Buffer tests passed");return 0;}
