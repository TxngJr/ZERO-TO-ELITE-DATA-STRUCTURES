#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint64_t hash_u64_mix(uint64_t value);
uint64_t hash_bytes_fnv1a(const void *data, size_t length);
uint64_t hash_c_string_fnv1a(const char *text);
bool hash_bucket(uint64_t hash, size_t bucket_count, size_t *out_index);

#endif
