#ifndef POSTING_LIST_H
#define POSTING_LIST_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct PostingList PostingList;
PostingList *posting_list_create(void);
void posting_list_free(PostingList *list);
size_t posting_list_size(const PostingList *list);
bool posting_list_add(PostingList *list,uint32_t doc_id);
bool posting_list_contains(const PostingList *list,uint32_t doc_id);
bool posting_list_get(const PostingList *list,size_t index,uint32_t *out_doc_id);
PostingList *posting_list_intersect(const PostingList *a,const PostingList *b);
bool posting_list_encode_gaps(const PostingList *list,uint8_t *out,size_t out_capacity,size_t *out_written);
PostingList *posting_list_decode_gaps(const uint8_t *data,size_t length);
bool posting_list_validate(const PostingList *list);
#endif
