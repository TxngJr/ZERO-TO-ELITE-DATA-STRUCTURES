#include "vector_index.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
enum { N=20000,D=8,K=10,QUERIES=100 };
typedef struct{size_t id;float d;} Ref;
static int cmp(const void*a,const void*b){const Ref*x=a,*y=b;if(x->d<y->d)return-1;if(x->d>y->d)return 1;return x->id<y->id?-1:(x->id>y->id?1:0);}
static float dist(const float*a,const float*b){double s=0;for(int i=0;i<D;++i){double x=(double)a[i]-b[i];s+=x*x;}return(float)s;}
int main(void){
    VectorIndex*idx=vi_create(D,4);assert(idx);
    float v[D];
    for(size_t i=0;i<N;++i){for(size_t d=0;d<D;++d)v[d]=(float)((i*17U+d*13U)%1009U)/100.0f+0.01f*(float)d;size_t id=SIZE_MAX;assert(vi_add(idx,v,&id)&&id==i);}
    assert(vi_size(idx)==N&&vi_dims(idx)==D&&vi_validate(idx));
    VectorSearchResult out[K];Ref*ref=malloc(N*sizeof(*ref));assert(ref);
    for(size_t q=0;q<QUERIES;++q){
        size_t source=(q*193U)%N;const float*query=vi_vector(idx,source);size_t count=0;assert(vi_search(idx,query,K,VECTOR_METRIC_L2,out,&count)&&count==K);
        for(size_t i=0;i<N;++i){ref[i].id=i;ref[i].d=dist(query,vi_vector(idx,i));}
        qsort(ref,N,sizeof(*ref),cmp);
        for(size_t i=0;i<K;++i){assert(out[i].id==ref[i].id);assert(fabsf(out[i].distance-ref[i].d)<1e-4f);}
    }
    const float*self=vi_vector(idx,1234);size_t count=0;assert(vi_search(idx,self,1,VECTOR_METRIC_COSINE,out,&count)&&count==1&&out[0].id==1234&&out[0].distance<1e-5f);
    float zero[D]={0};size_t bad=0;assert(!vi_add(idx,zero,&bad));
    float nanv[D]={1,1,1,1,1,1,1,1};nanv[3]=NAN;assert(!vi_add(idx,nanv,&bad));
    free(ref);vi_free(idx);puts("Vector index tests passed");return 0;
}
