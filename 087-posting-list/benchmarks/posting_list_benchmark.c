#include "posting_list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}int main(void){const uint32_t n=200000;PostingList*a=posting_list_create(),*b=posting_list_create();if(!a||!b)return 1;for(uint32_t i=0;i<n;++i){if(!posting_list_add(a,i*2U)||!posting_list_add(b,i*3U))return 1;}struct timespec s,t;timespec_get(&s,TIME_UTC);PostingList*x=posting_list_intersect(a,b);timespec_get(&t,TIME_UTC);if(!x)return 1;size_t bytes=0;if(!posting_list_encode_gaps(a,NULL,0,&bytes))return 1;printf("a=%zu b=%zu intersection=%zu encoded_a=%zu bytes intersect_seconds=%.9f\n",posting_list_size(a),posting_list_size(b),posting_list_size(x),bytes,e(s,t));posting_list_free(x);posting_list_free(b);posting_list_free(a);return 0;}
