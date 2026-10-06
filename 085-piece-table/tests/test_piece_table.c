#include "piece_table.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint32_t rng(uint32_t*s){*s=*s*1664525u+1013904223u;return*s;}
int main(void){enum{OPS=20000,MAX=18000};const uint8_t init[]="original text";PieceTable*t=piece_table_create(init,sizeof init-1);assert(t);uint8_t*model=malloc(MAX),*out=malloc(MAX);assert(model&&out);size_t n=sizeof init-1;memcpy(model,init,n);uint32_t s=0x85A12345u;
for(int op=0;op<OPS;++op){unsigned k=rng(&s)%3U;if((k<2&&n<MAX-4)||n==0){size_t pos=n?rng(&s)%(n+1):0;size_t len=1+rng(&s)%4U;if(n+len>MAX)len=MAX-n;uint8_t tmp[4];for(size_t j=0;j<len;++j)tmp[j]=(uint8_t)('a'+rng(&s)%26U);assert(piece_table_insert(t,pos,tmp,len));memmove(model+pos+len,model+pos,n-pos);memcpy(model+pos,tmp,len);n+=len;}else{size_t pos=rng(&s)%n;size_t len=1+rng(&s)%4U;if(len>n-pos)len=n-pos;assert(piece_table_erase(t,pos,len));memmove(model+pos,model+pos+len,n-pos-len);n-=len;}assert(piece_table_size(t)==n);assert(piece_table_validate(t));assert(piece_table_copy(t,out,MAX));assert(memcmp(model,out,n)==0);if(n){size_t i=rng(&s)%n;uint8_t v=0;assert(piece_table_get(t,i,&v)&&v==model[i]);}}
assert(piece_table_add_buffer_size(t)>0);free(out);free(model);piece_table_free(t);puts("Piece Table tests passed");return 0;}
