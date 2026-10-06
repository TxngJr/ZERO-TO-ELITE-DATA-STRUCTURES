#ifndef PIECE_TABLE_H
#define PIECE_TABLE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct PieceTable PieceTable;
PieceTable *piece_table_create(const uint8_t *original,size_t length);
void piece_table_free(PieceTable *table);
size_t piece_table_size(const PieceTable *table);
size_t piece_table_piece_count(const PieceTable *table);
size_t piece_table_add_buffer_size(const PieceTable *table);
bool piece_table_insert(PieceTable *table,size_t position,const uint8_t *bytes,size_t length);
bool piece_table_erase(PieceTable *table,size_t position,size_t length);
bool piece_table_get(const PieceTable *table,size_t index,uint8_t *out_byte);
bool piece_table_copy(const PieceTable *table,uint8_t *out,size_t out_capacity);
bool piece_table_validate(const PieceTable *table);
#endif
