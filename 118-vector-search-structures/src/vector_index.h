#ifndef VECTOR_INDEX_H
#define VECTOR_INDEX_H
#include <stdbool.h>
#include <stddef.h>
typedef struct VectorIndex VectorIndex;
typedef enum { VECTOR_METRIC_L2=0, VECTOR_METRIC_COSINE=1 } VectorMetric;
typedef struct { size_t id; float distance; } VectorSearchResult;
VectorIndex *vi_create(size_t dims,size_t initial_capacity);
void vi_free(VectorIndex *index);
bool vi_add(VectorIndex *index,const float *vector,size_t *out_id);
bool vi_search(const VectorIndex *index,const float *query,size_t k,VectorMetric metric,VectorSearchResult *out_results,size_t *out_count);
size_t vi_size(const VectorIndex *index);
size_t vi_dims(const VectorIndex *index);
const float *vi_vector(const VectorIndex *index,size_t id);
bool vi_validate(const VectorIndex *index);
#endif
