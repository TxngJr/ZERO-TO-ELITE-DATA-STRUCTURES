#include "posting_list.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int main(void){PostingList*a=posting_list_create(),*b=posting_list_create();assert(a&&b);for(uint32_t i=0;i<10000;i+=2)assert(posting_list_add(a,i));for(uint32_t i=0;i<10000;i+=3)assert(posting_list_add(b,i));assert(posting_list_add(a,42));assert(posting_list_size(a)==5000);PostingList*x=posting_list_intersect(a,b);assert(x);size_t expected=0;for(uint32_t i=0;i<10000;i+=6){uint32_t d=0;assert(posting_list_get(x,expected,&d)&&d==i);++expected;}assert(posting_list_size(x)==expected);
size_t need=0;assert(posting_list_encode_gaps(a,NULL,0,&need));uint8_t*buf=malloc(need?need:1);assert(buf);size_t written=0;assert(posting_list_encode_gaps(a,buf,need,&written)&&written==need);PostingList*decoded=posting_list_decode_gaps(buf,written);assert(decoded);assert(posting_list_size(decoded)==posting_list_size(a));for(size_t i=0;i<posting_list_size(a);++i){uint32_t da=0,db=0;assert(posting_list_get(a,i,&da)&&posting_list_get(decoded,i,&db)&&da==db);}assert(posting_list_validate(a)&&posting_list_validate(b)&&posting_list_validate(x)&&posting_list_validate(decoded));
const uint8_t bad1[]={0x80};assert(posting_list_decode_gaps(bad1,sizeof bad1)==NULL);const uint8_t bad2[]={0xff,0xff,0xff,0xff,0x1f};assert(posting_list_decode_gaps(bad2,sizeof bad2)==NULL);free(buf);posting_list_free(decoded);posting_list_free(x);posting_list_free(b);posting_list_free(a);puts("Posting List tests passed");return 0;}
