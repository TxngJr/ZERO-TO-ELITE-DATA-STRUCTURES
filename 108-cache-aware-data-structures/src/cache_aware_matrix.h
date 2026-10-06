#ifndef CACHE_AWARE_MATRIX_H
#define CACHE_AWARE_MATRIX_H
#include <stdbool.h>
#include <stddef.h>
typedef struct CacheAwareMatrix CacheAwareMatrix;
CacheAwareMatrix *cam_create(size_t rows,size_t cols,size_t tile_rows,size_t tile_cols);
void cam_free(CacheAwareMatrix *matrix);
size_t cam_rows(const CacheAwareMatrix *matrix);
size_t cam_cols(const CacheAwareMatrix *matrix);
size_t cam_tile_rows(const CacheAwareMatrix *matrix);
size_t cam_tile_cols(const CacheAwareMatrix *matrix);
bool cam_set(CacheAwareMatrix *matrix,size_t row,size_t col,double value);
bool cam_get(const CacheAwareMatrix *matrix,size_t row,size_t col,double *out_value);
bool cam_fill_from_dense(CacheAwareMatrix *matrix,const double *dense,size_t count);
bool cam_copy_to_dense(const CacheAwareMatrix *matrix,double *dense,size_t count);
double cam_sum_row_order(const CacheAwareMatrix *matrix);
double cam_sum_tile_order(const CacheAwareMatrix *matrix);
bool cam_transpose_to_dense(const CacheAwareMatrix *matrix,double *out,size_t count);
bool cam_validate(const CacheAwareMatrix *matrix);
#endif
