#include "vector_index.h"
#include <stdio.h>
int main(void){VectorIndex*i=vi_create(3,4);if(!i)return 1;float a[3]={1,0,0},b[3]={0,1,0},q[3]={.9f,.1f,.01f};size_t id;if(!vi_add(i,a,&id)||!vi_add(i,b,&id))return 2;VectorSearchResult r[2];size_t n=0;if(!vi_search(i,q,2,VECTOR_METRIC_L2,r,&n))return 3;for(size_t k=0;k<n;++k)printf("id=%zu distance=%.4f\n",r[k].id,r[k].distance);vi_free(i);return 0;}
