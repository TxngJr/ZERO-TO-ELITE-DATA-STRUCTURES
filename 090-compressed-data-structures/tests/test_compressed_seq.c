#include "u64_compressed_seq.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
static size_t naive_lb(const uint64_t*v,size_t n,uint64_t x){size_t l=0,h=n;while(l<h){size_t m=l+(h-l)/2;if(v[m]<x)l=m+1;else h=m;}return l;}
int main(void){const size_t n=100000;uint64_t*v=malloc(n*sizeof*v);assert(v);uint64_t x=7;for(size_t i=0;i<n;++i){x+=1+(i%17==0?1000:(i%5));v[i]=x;}U64CompressedSeq*s=u64_compressed_seq_create(v,n,128);assert(s);assert(u64_compressed_seq_validate(s));assert(u64_compressed_seq_size(s)==n);for(size_t i=0;i<n;i+=97){uint64_t got=0;assert(u64_compressed_seq_get(s,i,&got)&&got==v[i]);}for(uint64_t q=0;q<v[n-1]+1000;q+=137){assert(u64_compressed_seq_lower_bound(s,q)==naive_lb(v,n,q));}assert(u64_compressed_seq_encoded_bytes(s)<n*sizeof(uint64_t));uint64_t bad[]={1,2,2};assert(u64_compressed_seq_create(bad,3,4)==NULL);u64_compressed_seq_free(s);free(v);puts("Compressed Data Structure tests passed");return 0;}
