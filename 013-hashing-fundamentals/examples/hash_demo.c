#include "hash_functions.h"

#include <inttypes.h>
#include <stdio.h>

int main(void) {
    const char *words[] = {"cat", "dog", "cot", "tac"};

    for (size_t i = 0; i < sizeof words / sizeof words[0]; ++i) {
        const uint64_t h = hash_c_string_fnv1a(words[i]);
        size_t bucket = 0;
        hash_bucket(h, 8, &bucket);
        printf("%s -> 0x%016" PRIx64 " -> bucket %zu\n", words[i], h, bucket);
    }

    puts("integer multiples of 16 mapped to 16 buckets:");

    for (uint64_t x = 0; x < 80; x += 16) {
        size_t direct = (size_t)(x % 16);
        size_t mixed = 0;
        hash_bucket(hash_u64_mix(x), 16, &mixed);
        printf("%" PRIu64 ": direct=%zu mixed=%zu\n", x, direct, mixed);
    }

    return 0;
}
