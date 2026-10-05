#ifndef BYTE_CUCKOO_FILTER_H
#define BYTE_CUCKOO_FILTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct ByteCuckooFilter ByteCuckooFilter;

ByteCuckooFilter *byte_cuckoo_filter_create(size_t bucket_count);
void byte_cuckoo_filter_free(ByteCuckooFilter *filter);

size_t byte_cuckoo_filter_size(const ByteCuckooFilter *filter);
size_t byte_cuckoo_filter_bucket_count(const ByteCuckooFilter *filter);

bool byte_cuckoo_filter_add(ByteCuckooFilter *filter,const uint8_t *key,size_t length);
bool byte_cuckoo_filter_remove(ByteCuckooFilter *filter,const uint8_t *key,size_t length);
bool byte_cuckoo_filter_maybe_contains(const ByteCuckooFilter *filter,const uint8_t *key,size_t length,bool *out_maybe);
bool byte_cuckoo_filter_validate(const ByteCuckooFilter *filter);

#endif
