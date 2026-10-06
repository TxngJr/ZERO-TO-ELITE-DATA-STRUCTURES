#ifndef TENSOR_LAYOUT_H
#define TENSOR_LAYOUT_H
#include <stdbool.h>
#include <stddef.h>
#define TENSOR_LAYOUT_MAX_DIMS 8
typedef struct{size_t ndim;size_t shape[TENSOR_LAYOUT_MAX_DIMS];size_t stride[TENSOR_LAYOUT_MAX_DIMS];size_t offset;}TensorLayout;
bool tensor_layout_row_major(TensorLayout *out,const size_t *shape,size_t ndim);
bool tensor_layout_column_major(TensorLayout *out,const size_t *shape,size_t ndim);
bool tensor_layout_offset(const TensorLayout *layout,const size_t *index,size_t *out_offset);
bool tensor_layout_permute(const TensorLayout *src,const size_t *perm,TensorLayout *out);
bool tensor_layout_slice(const TensorLayout *src,size_t dim,size_t start,size_t length,size_t step,TensorLayout *out);
bool tensor_layout_is_row_major_contiguous(const TensorLayout *layout);
bool tensor_layout_is_column_major_contiguous(const TensorLayout *layout);
bool tensor_layout_validate(const TensorLayout *layout);
bool tensor_layout_total_elements(const TensorLayout *layout,size_t *out_total);
#endif
