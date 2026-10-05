#include "hash_functions.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    assert(hash_u64_mix(123) == hash_u64_mix(123));
    assert(hash_u64_mix(123) != hash_u64_mix(124));

    const char bytes[] = {'a', '\0', 'b'};
    const uint64_t full = hash_bytes_fnv1a(bytes, sizeof bytes);
    const uint64_t prefix = hash_bytes_fnv1a(bytes, 1);
    assert(full != prefix);

    assert(hash_c_string_fnv1a("hello") == hash_bytes_fnv1a("hello", 5));
    assert(hash_c_string_fnv1a("hello") != hash_c_string_fnv1a("world"));

    size_t bucket = 999;
    assert(hash_bucket(UINT64_C(123456), 17, &bucket));
    assert(bucket < 17);
    assert(!hash_bucket(5, 0, &bucket));
    assert(!hash_bucket(5, 10, NULL));

    assert(hash_bytes_fnv1a(NULL, 0) == UINT64_C(14695981039346656037));

    puts("hash function tests passed");
    return 0;
}
