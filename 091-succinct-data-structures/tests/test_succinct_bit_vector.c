#include "succinct_bit_vector.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t rng_state = UINT64_C(0x9e3779b97f4a7c15);
static uint64_t rng64(void) {
    uint64_t x = rng_state;
    x ^= x << 7; x ^= x >> 9; x ^= x << 8;
    return rng_state = x;
}

static size_t naive_rank1(const uint8_t *b, size_t end) {
    size_t r = 0;
    for (size_t i = 0; i < end; ++i) r += b[i];
    return r;
}
static size_t naive_select(const uint8_t *b, size_t n, uint8_t want, size_t kth) {
    for (size_t i = 0; i < n; ++i) if (b[i] == want) {
        if (kth == 0) return i;
        --kth;
    }
    return n;
}

static void boundary_case(size_t n) {
    uint8_t *bits = n ? malloc(n) : NULL;
    assert(n == 0 || bits);
    for (size_t i = 0; i < n; ++i) bits[i] = (uint8_t)(((i * 17U + 3U) % 11U) < 4U);
    SuccinctBitVector *bv = sbv_create(bits, n);
    assert(bv && sbv_validate(bv) && sbv_size(bv) == n);
    for (size_t end = 0; end <= n; ++end) {
        size_t r1 = 0, r0 = 0;
        assert(sbv_rank1(bv, end, &r1) && r1 == naive_rank1(bits, end));
        assert(sbv_rank0(bv, end, &r0) && r0 == end - r1);
    }
    size_t ones = naive_rank1(bits, n), zeros = n - ones;
    for (size_t k = 0; k < ones; ++k) {
        size_t got = n;
        assert(sbv_select1(bv, k, &got));
        assert(got == naive_select(bits, n, 1, k));
    }
    for (size_t k = 0; k < zeros; ++k) {
        size_t got = n;
        assert(sbv_select0(bv, k, &got));
        assert(got == naive_select(bits, n, 0, k));
    }
    bool bit = false;
    if (n) { assert(sbv_get(bv, n - 1U, &bit)); assert(bit == (bits[n-1U] != 0)); }
    assert(!sbv_get(bv, n, &bit));
    sbv_free(bv); free(bits);
}

int main(void) {
    size_t sizes[] = {0,1,2,63,64,65,511,512,513,1023,1024,1025};
    for (size_t i = 0; i < sizeof sizes / sizeof sizes[0]; ++i) boundary_case(sizes[i]);

    const size_t n = 200000;
    uint8_t *bits = malloc(n);
    assert(bits);
    size_t ones = 0;
    for (size_t i = 0; i < n; ++i) { bits[i] = (uint8_t)((rng64() % 7U) < 3U); ones += bits[i]; }
    SuccinctBitVector *bv = sbv_create(bits, n);
    assert(bv && sbv_validate(bv) && sbv_ones(bv) == ones);
    for (size_t q = 0; q < 5000; ++q) {
        size_t end = (size_t)(rng64() % (n + 1U));
        size_t got = 0;
        assert(sbv_rank1(bv, end, &got) && got == naive_rank1(bits, end));
    }
    for (size_t q = 0; q < 5000 && ones; ++q) {
        size_t k = (size_t)(rng64() % ones), got = n;
        assert(sbv_select1(bv, k, &got) && got == naive_select(bits, n, 1, k));
    }
    size_t zeros = n - ones;
    for (size_t q = 0; q < 5000 && zeros; ++q) {
        size_t k = (size_t)(rng64() % zeros), got = n;
        assert(sbv_select0(bv, k, &got) && got == naive_select(bits, n, 0, k));
    }

    uint8_t bad[] = {0,1,2,0};
    assert(sbv_create(bad, 4) == NULL);
    sbv_free(bv); free(bits);
    puts("Succinct bit-vector tests passed");
    return 0;
}
