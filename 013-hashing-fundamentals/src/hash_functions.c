#include "hash_functions.h"

#include <string.h>

uint64_t hash_u64_mix(uint64_t value) {
    value ^= value >> 30;
    value *= UINT64_C(0xbf58476d1ce4e5b9);
    value ^= value >> 27;
    value *= UINT64_C(0x94d049bb133111eb);
    value ^= value >> 31;
    return value;
}

uint64_t hash_bytes_fnv1a(const void *data, size_t length) {
    const unsigned char *bytes = data;
    uint64_t hash = UINT64_C(14695981039346656037);

    if (bytes == NULL && length != 0) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        hash ^= (uint64_t)bytes[i];
        hash *= UINT64_C(1099511628211);
    }

    return hash;
}

uint64_t hash_c_string_fnv1a(const char *text) {
    if (text == NULL) return 0;
    return hash_bytes_fnv1a(text, strlen(text));
}

bool hash_bucket(uint64_t hash, size_t bucket_count, size_t *out_index) {
    if (bucket_count == 0 || out_index == NULL) return false;
    *out_index = (size_t)(hash % bucket_count);
    return true;
}
