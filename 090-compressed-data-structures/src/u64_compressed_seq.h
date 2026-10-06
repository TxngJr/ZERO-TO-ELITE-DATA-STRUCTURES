#ifndef U64_COMPRESSED_SEQ_H
#define U64_COMPRESSED_SEQ_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct U64CompressedSeq U64CompressedSeq;
U64CompressedSeq *u64_compressed_seq_create(const uint64_t *values,size_t count,size_t block_size);
void u64_compressed_seq_free(U64CompressedSeq *seq);
size_t u64_compressed_seq_size(const U64CompressedSeq *seq);
size_t u64_compressed_seq_block_size(const U64CompressedSeq *seq);
size_t u64_compressed_seq_block_count(const U64CompressedSeq *seq);
size_t u64_compressed_seq_encoded_bytes(const U64CompressedSeq *seq);
bool u64_compressed_seq_get(const U64CompressedSeq *seq,size_t index,uint64_t *out_value);
size_t u64_compressed_seq_lower_bound(const U64CompressedSeq *seq,uint64_t target);
bool u64_compressed_seq_validate(const U64CompressedSeq *seq);
#endif
