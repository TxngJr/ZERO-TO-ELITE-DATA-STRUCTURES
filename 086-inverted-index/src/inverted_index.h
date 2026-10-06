#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct InvertedIndex InvertedIndex;
InvertedIndex *inverted_index_create(size_t bucket_count);
void inverted_index_free(InvertedIndex *index);
size_t inverted_index_term_count(const InvertedIndex *index);
bool inverted_index_add_document(InvertedIndex *index,uint64_t doc_id,const char *text);
size_t inverted_index_doc_freq(const InvertedIndex *index,const char *term);
bool inverted_index_get_doc(const InvertedIndex *index,const char *term,size_t occurrence,uint64_t *out_doc_id);
size_t inverted_index_and(const InvertedIndex *index,const char *term_a,const char *term_b,uint64_t *out,size_t out_capacity);
bool inverted_index_validate(const InvertedIndex *index);
#endif
