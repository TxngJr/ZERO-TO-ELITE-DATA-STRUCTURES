#ifndef ML_STRUCTURES_H
#define ML_STRUCTURES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct MlDataset MlDataset;
typedef struct BatchPlan BatchPlan;
typedef struct EmbeddingTable EmbeddingTable;
MlDataset *ml_dataset_create(size_t rows,size_t features);
void ml_dataset_free(MlDataset *dataset);
bool ml_dataset_set_row(MlDataset *dataset,size_t row,const float *features,int label);
const float *ml_dataset_row(const MlDataset *dataset,size_t row);
bool ml_dataset_label(const MlDataset *dataset,size_t row,int *out_label);
size_t ml_dataset_rows(const MlDataset *dataset);
size_t ml_dataset_features(const MlDataset *dataset);
bool ml_dataset_gather(const MlDataset *dataset,const size_t *indices,size_t count,float *out_features,int *out_labels);
bool ml_dataset_validate(const MlDataset *dataset);
BatchPlan *batch_plan_create(size_t rows,size_t batch_size,uint64_t seed);
void batch_plan_free(BatchPlan *plan);
bool batch_plan_next(BatchPlan *plan,const size_t **out_indices,size_t *out_count,bool *out_has_batch);
bool batch_plan_reset(BatchPlan *plan,uint64_t seed,bool reshuffle);
size_t batch_plan_batch_size(const BatchPlan *plan);
bool batch_plan_validate(const BatchPlan *plan);
EmbeddingTable *embedding_create(size_t rows,size_t dims);
void embedding_free(EmbeddingTable *table);
bool embedding_set_row(EmbeddingTable *table,size_t row,const float *values);
const float *embedding_row(const EmbeddingTable *table,size_t row);
bool embedding_gather(const EmbeddingTable *table,const size_t *indices,size_t count,float *out_values);
size_t embedding_rows(const EmbeddingTable *table);
size_t embedding_dims(const EmbeddingTable *table);
bool embedding_validate(const EmbeddingTable *table);
#endif
