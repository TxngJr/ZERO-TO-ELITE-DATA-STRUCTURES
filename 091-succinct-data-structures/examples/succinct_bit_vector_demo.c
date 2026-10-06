#include "succinct_bit_vector.h"
#include <stdio.h>
int main(void){uint8_t b[]={1,0,1,1,0,0,1};SuccinctBitVector*v=sbv_create(b,7);if(!v)return 1;size_t r=0,s=0;sbv_rank1(v,5,&r);sbv_select1(v,2,&s);printf("rank1([0,5))=%zu select1(2)=%zu\n",r,s);sbv_free(v);return 0;}
