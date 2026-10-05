#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct GapBuffer GapBuffer;
GapBuffer *gap_buffer_create(size_t initial_gap);
void gap_buffer_free(GapBuffer *buffer);
size_t gap_buffer_size(const GapBuffer *buffer);
size_t gap_buffer_capacity(const GapBuffer *buffer);
size_t gap_buffer_gap_position(const GapBuffer *buffer);
size_t gap_buffer_gap_size(const GapBuffer *buffer);
bool gap_buffer_insert(GapBuffer *buffer,size_t position,const uint8_t *bytes,size_t length);
bool gap_buffer_erase(GapBuffer *buffer,size_t position,size_t length);
bool gap_buffer_get(const GapBuffer *buffer,size_t index,uint8_t *out_byte);
bool gap_buffer_copy(const GapBuffer *buffer,uint8_t *out,size_t out_capacity);
bool gap_buffer_validate(const GapBuffer *buffer);
#endif
