#include "vector_index.h"
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct VectorIndex {
    size_t dims;
    size_t size;
    size_t capacity;
    float *vectors;
    float *norm_sq;
};

static bool vector_norm_sq(const float *v,size_t dims,float *out) {
    double sum=0.0;
    for(size_t i=0;i<dims;++i){
        if(!isfinite(v[i])) return false;
        double x=v[i];
        sum+=x*x;
    }
    if(!(sum>0.0) || !isfinite(sum) || sum>(double)FLT_MAX) return false;
    *out=(float)sum;
    return true;
}

VectorIndex *vi_create(size_t dims,size_t initial_capacity) {
    if(dims==0U) return NULL;
    size_t capacity=initial_capacity?initial_capacity:16U;
    if(capacity>SIZE_MAX/dims || capacity*dims>SIZE_MAX/sizeof(float) ||
       capacity>SIZE_MAX/sizeof(float)) return NULL;

    VectorIndex *index=calloc(1,sizeof(*index));
    if(!index) return NULL;
    index->vectors=malloc(capacity*dims*sizeof(*index->vectors));
    index->norm_sq=malloc(capacity*sizeof(*index->norm_sq));
    if(!index->vectors || !index->norm_sq){vi_free(index);return NULL;}
    index->dims=dims;
    index->capacity=capacity;
    return index;
}

void vi_free(VectorIndex *index){
    if(!index)return;
    free(index->vectors);
    free(index->norm_sq);
    free(index);
}

static bool vi_grow(VectorIndex *index){
    if(index->capacity>SIZE_MAX/2U) return false;
    size_t new_capacity=index->capacity*2U;
    if(new_capacity>SIZE_MAX/index->dims ||
       new_capacity*index->dims>SIZE_MAX/sizeof(float) ||
       new_capacity>SIZE_MAX/sizeof(float)) return false;

    float *new_vectors=malloc(new_capacity*index->dims*sizeof(*new_vectors));
    float *new_norms=malloc(new_capacity*sizeof(*new_norms));
    if(!new_vectors || !new_norms){
        free(new_vectors);
        free(new_norms);
        return false;
    }

    memcpy(new_vectors,index->vectors,index->size*index->dims*sizeof(*new_vectors));
    memcpy(new_norms,index->norm_sq,index->size*sizeof(*new_norms));
    free(index->vectors);
    free(index->norm_sq);
    index->vectors=new_vectors;
    index->norm_sq=new_norms;
    index->capacity=new_capacity;
    return true;
}

bool vi_add(VectorIndex *index,const float *vector,size_t *out_id){
    if(!index || !vector || !out_id) return false;
    float norm=0.0f;
    if(!vector_norm_sq(vector,index->dims,&norm)) return false;
    if(index->size==index->capacity && !vi_grow(index)) return false;

    size_t id=index->size;
    memcpy(index->vectors+id*index->dims,vector,index->dims*sizeof(float));
    index->norm_sq[id]=norm;
    ++index->size;
    *out_id=id;
    return true;
}

static bool result_worse(VectorSearchResult a,VectorSearchResult b){
    return a.distance>b.distance ||
           (a.distance==b.distance && a.id>b.id);
}

static void heap_sift_up(VectorSearchResult *heap,size_t pos){
    while(pos>0U){
        size_t parent=(pos-1U)/2U;
        if(!result_worse(heap[pos],heap[parent])) break;
        VectorSearchResult tmp=heap[pos];
        heap[pos]=heap[parent];
        heap[parent]=tmp;
        pos=parent;
    }
}

static void heap_sift_down(VectorSearchResult *heap,size_t count,size_t pos){
    for(;;){
        size_t left=pos*2U+1U;
        if(left>=count) break;
        size_t worst=left;
        size_t right=left+1U;
        if(right<count && result_worse(heap[right],heap[left])) worst=right;
        if(!result_worse(heap[worst],heap[pos])) break;
        VectorSearchResult tmp=heap[pos];
        heap[pos]=heap[worst];
        heap[worst]=tmp;
        pos=worst;
    }
}

static int result_compare(const void *ap,const void *bp){
    const VectorSearchResult *a=ap,*b=bp;
    if(a->distance<b->distance) return -1;
    if(a->distance>b->distance) return 1;
    return a->id<b->id?-1:(a->id>b->id?1:0);
}

bool vi_search(const VectorIndex *index,const float *query,size_t k,
               VectorMetric metric,VectorSearchResult *out_results,
               size_t *out_count){
    if(!index || !query || !out_count || (k&& !out_results) ||
       (metric!=VECTOR_METRIC_L2 && metric!=VECTOR_METRIC_COSINE)) return false;

    float query_norm=0.0f;
    if(!vector_norm_sq(query,index->dims,&query_norm)) return false;
    size_t wanted=k<index->size?k:index->size;
    if(wanted==0U){*out_count=0U;return true;}

    size_t heap_count=0U;
    for(size_t id=0;id<index->size;++id){
        const float *v=index->vectors+id*index->dims;
        double score=0.0;
        if(metric==VECTOR_METRIC_L2){
            for(size_t d=0;d<index->dims;++d){
                double delta=(double)query[d]-(double)v[d];
                score+=delta*delta;
            }
        }else{
            double dot=0.0;
            for(size_t d=0;d<index->dims;++d) dot+=(double)query[d]*(double)v[d];
            double denom=sqrt((double)query_norm*(double)index->norm_sq[id]);
            score=1.0-dot/denom;
            if(score<0.0 && score>-1e-7) score=0.0;
        }
        if(!isfinite(score) || score>(double)FLT_MAX) return false;

        VectorSearchResult candidate={id,(float)score};
        if(heap_count<wanted){
            out_results[heap_count]=candidate;
            heap_sift_up(out_results,heap_count);
            ++heap_count;
        }else if(result_worse(out_results[0],candidate)){
            out_results[0]=candidate;
            heap_sift_down(out_results,heap_count,0U);
        }
    }

    qsort(out_results,heap_count,sizeof(*out_results),result_compare);
    *out_count=heap_count;
    return true;
}

size_t vi_size(const VectorIndex *index){return index?index->size:0U;}
size_t vi_dims(const VectorIndex *index){return index?index->dims:0U;}
const float *vi_vector(const VectorIndex *index,size_t id){
    return index&&id<index->size?index->vectors+id*index->dims:NULL;
}

bool vi_validate(const VectorIndex *index){
    if(!index || !index->vectors || !index->norm_sq || index->dims==0U ||
       index->capacity==0U || index->size>index->capacity) return false;
    for(size_t id=0;id<index->size;++id){
        float expected=0.0f;
        if(!vector_norm_sq(index->vectors+id*index->dims,index->dims,&expected)) return false;
        float diff=fabsf(expected-index->norm_sq[id]);
        float scale=fmaxf(1.0f,expected);
        if(diff>1e-5f*scale) return false;
    }
    return true;
}
