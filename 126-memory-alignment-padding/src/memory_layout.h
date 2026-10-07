#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H
#include <stdbool.h>
#include <stddef.h>
typedef struct {
    size_t size;
    size_t alignment;
} MaField;
typedef struct {
    size_t field_count;
    size_t struct_size;
    size_t struct_alignment;
    size_t padding_bytes;
    size_t offsets[32];
} MaLayout;
typedef struct MaAlignedArray MaAlignedArray;
bool ma_align_up(size_t value,size_t alignment,size_t *out);
bool ma_compute_layout(const MaField *fields,size_t field_count,MaLayout *out);
MaAlignedArray *ma_aligned_array_create(size_t count,size_t element_size,size_t alignment);
void ma_aligned_array_free(MaAlignedArray *array);
void *ma_aligned_array_get(MaAlignedArray *array,size_t index);
const void *ma_aligned_array_get_const(const MaAlignedArray *array,size_t index);
size_t ma_aligned_array_count(const MaAlignedArray *array);
size_t ma_aligned_array_element_size(const MaAlignedArray *array);
size_t ma_aligned_array_alignment(const MaAlignedArray *array);
size_t ma_aligned_array_stride(const MaAlignedArray *array);
size_t ma_aligned_array_storage_bytes(const MaAlignedArray *array);
bool ma_aligned_array_validate(const MaAlignedArray *array);
#endif
