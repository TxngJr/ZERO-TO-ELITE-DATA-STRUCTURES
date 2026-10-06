#ifndef CACHE_OBLIVIOUS_MATRIX_H
#define CACHE_OBLIVIOUS_MATRIX_H
#include <stdbool.h>
#include <stddef.h>
typedef struct CacheObliviousMatrix CacheObliviousMatrix;
CacheObliviousMatrix *com_create(size_t rows,size_t cols);
void com_free(CacheObliviousMatrix *matrix);
size_t com_rows(const CacheObliviousMatrix *matrix);
size_t com_cols(const CacheObliviousMatrix *matrix);
size_t com_padded_side(const CacheObliviousMatrix *matrix);
bool com_set(CacheObliviousMatrix *matrix,size_t row,size_t col,double value);
bool com_get(const CacheObliviousMatrix *matrix,size_t row,size_t col,double *out);
bool com_copy_to_dense(const CacheObliviousMatrix *matrix,double *out,size_t count);
double com_sum_row_order(const CacheObliviousMatrix *matrix);
double com_sum_z_order(const CacheObliviousMatrix *matrix);
bool com_transpose_to_dense(const CacheObliviousMatrix *matrix,double *out,size_t count);
bool com_validate(const CacheObliviousMatrix *matrix);
#endif
