#include "succinct_bit_vector.h"
#include <stdlib.h>

enum { WORD_BITS = 64, SUPER_WORDS = 8 };

struct SuccinctBitVector {
    size_t nbits;
    size_t word_count;
    size_t super_count;
    size_t ones;
    uint64_t *words;
    size_t *super_rank;
};

static size_t popcount64(uint64_t x) {
    size_t c = 0;
    while (x) {
        x &= x - 1U;
        ++c;
    }
    return c;
}

static uint64_t valid_mask(size_t valid_bits) {
    if (valid_bits >= WORD_BITS) return UINT64_MAX;
    if (valid_bits == 0) return UINT64_C(0);
    return (UINT64_C(1) << valid_bits) - UINT64_C(1);
}

static size_t words_in_super(const SuccinctBitVector *bv, size_t s) {
    size_t start = s * (size_t)SUPER_WORDS;
    size_t remain = bv->word_count - start;
    return remain < (size_t)SUPER_WORDS ? remain : (size_t)SUPER_WORDS;
}

SuccinctBitVector *sbv_create(const uint8_t *bits, size_t nbits) {
    if (nbits > 0 && !bits) return NULL;
    if (nbits > SIZE_MAX - (WORD_BITS - 1U)) return NULL;

    size_t word_count = (nbits + (WORD_BITS - 1U)) / WORD_BITS;
    size_t super_count = word_count == 0 ? 0 : (word_count + SUPER_WORDS - 1U) / SUPER_WORDS;

    if (word_count > SIZE_MAX / sizeof(uint64_t)) return NULL;
    if (super_count == SIZE_MAX || (super_count + 1U) > SIZE_MAX / sizeof(size_t)) return NULL;

    SuccinctBitVector *bv = calloc(1, sizeof(*bv));
    if (!bv) return NULL;
    bv->nbits = nbits;
    bv->word_count = word_count;
    bv->super_count = super_count;

    if (word_count) {
        bv->words = calloc(word_count, sizeof(*bv->words));
        if (!bv->words) {
            sbv_free(bv);
            return NULL;
        }
    }
    bv->super_rank = calloc(super_count + 1U, sizeof(*bv->super_rank));
    if (!bv->super_rank) {
        sbv_free(bv);
        return NULL;
    }

    for (size_t i = 0; i < nbits; ++i) {
        if (bits[i] > 1U) {
            sbv_free(bv);
            return NULL;
        }
        if (bits[i]) bv->words[i / WORD_BITS] |= UINT64_C(1) << (i % WORD_BITS);
    }

    size_t running = 0;
    for (size_t s = 0; s < super_count; ++s) {
        bv->super_rank[s] = running;
        size_t start = s * (size_t)SUPER_WORDS;
        size_t count = words_in_super(bv, s);
        for (size_t j = 0; j < count; ++j) running += popcount64(bv->words[start + j]);
    }
    bv->super_rank[super_count] = running;
    bv->ones = running;
    return bv;
}

void sbv_free(SuccinctBitVector *bv) {
    if (!bv) return;
    free(bv->words);
    free(bv->super_rank);
    free(bv);
}

size_t sbv_size(const SuccinctBitVector *bv) { return bv ? bv->nbits : 0; }
size_t sbv_ones(const SuccinctBitVector *bv) { return bv ? bv->ones : 0; }

size_t sbv_storage_bytes(const SuccinctBitVector *bv) {
    if (!bv) return 0;
    if (bv->word_count > (SIZE_MAX - sizeof(*bv)) / sizeof(uint64_t)) return SIZE_MAX;
    size_t total = sizeof(*bv) + bv->word_count * sizeof(uint64_t);
    size_t dirs = bv->super_count + 1U;
    if (dirs > (SIZE_MAX - total) / sizeof(size_t)) return SIZE_MAX;
    return total + dirs * sizeof(size_t);
}

bool sbv_get(const SuccinctBitVector *bv, size_t index, bool *out_bit) {
    if (!bv || !out_bit || index >= bv->nbits) return false;
    *out_bit = ((bv->words[index / WORD_BITS] >> (index % WORD_BITS)) & UINT64_C(1)) != 0;
    return true;
}

bool sbv_rank1(const SuccinctBitVector *bv, size_t end, size_t *out_rank) {
    if (!bv || !out_rank || end > bv->nbits) return false;
    if (end == 0) {
        *out_rank = 0;
        return true;
    }
    size_t full_words = end / WORD_BITS;
    size_t rem = end % WORD_BITS;
    if (full_words == bv->word_count && rem == 0) {
        *out_rank = bv->ones;
        return true;
    }

    size_t s = full_words / SUPER_WORDS;
    size_t rank = bv->super_rank[s];
    size_t start = s * (size_t)SUPER_WORDS;
    for (size_t w = start; w < full_words; ++w) rank += popcount64(bv->words[w]);
    if (rem) rank += popcount64(bv->words[full_words] & valid_mask(rem));
    *out_rank = rank;
    return true;
}

bool sbv_rank0(const SuccinctBitVector *bv, size_t end, size_t *out_rank) {
    size_t r1 = 0;
    if (!out_rank || !sbv_rank1(bv, end, &r1)) return false;
    *out_rank = end - r1;
    return true;
}

static bool select_in_word(uint64_t word, size_t kth, size_t *out_bit) {
    for (size_t bit = 0; bit < WORD_BITS; ++bit) {
        if ((word >> bit) & UINT64_C(1)) {
            if (kth == 0) {
                *out_bit = bit;
                return true;
            }
            --kth;
        }
    }
    return false;
}

bool sbv_select1(const SuccinctBitVector *bv, size_t kth, size_t *out_index) {
    if (!bv || !out_index || kth >= bv->ones) return false;

    size_t lo = 0, hi = bv->super_count;
    while (lo + 1U < hi) {
        size_t mid = lo + (hi - lo) / 2U;
        if (bv->super_rank[mid] <= kth) lo = mid;
        else hi = mid;
    }
    size_t s = lo;
    size_t remain = kth - bv->super_rank[s];
    size_t start = s * (size_t)SUPER_WORDS;
    size_t count = words_in_super(bv, s);
    for (size_t j = 0; j < count; ++j) {
        size_t pc = popcount64(bv->words[start + j]);
        if (remain < pc) {
            size_t bit = 0;
            if (!select_in_word(bv->words[start + j], remain, &bit)) return false;
            *out_index = (start + j) * (size_t)WORD_BITS + bit;
            return *out_index < bv->nbits;
        }
        remain -= pc;
    }
    return false;
}

bool sbv_select0(const SuccinctBitVector *bv, size_t kth, size_t *out_index) {
    if (!bv || !out_index) return false;
    size_t zeros = bv->nbits - bv->ones;
    if (kth >= zeros) return false;

    size_t lo = 0, hi = bv->super_count;
    while (lo + 1U < hi) {
        size_t mid = lo + (hi - lo) / 2U;
        size_t bits_before = mid * (size_t)SUPER_WORDS * (size_t)WORD_BITS;
        if (bits_before > bv->nbits) bits_before = bv->nbits;
        size_t zeros_before = bits_before - bv->super_rank[mid];
        if (zeros_before <= kth) lo = mid;
        else hi = mid;
    }
    size_t s = lo;
    size_t bits_before = s * (size_t)SUPER_WORDS * (size_t)WORD_BITS;
    size_t zeros_before = bits_before - bv->super_rank[s];
    size_t remain = kth - zeros_before;
    size_t start = s * (size_t)SUPER_WORDS;
    size_t count = words_in_super(bv, s);

    for (size_t j = 0; j < count; ++j) {
        size_t word_index = start + j;
        size_t valid = WORD_BITS;
        if (word_index + 1U == bv->word_count && (bv->nbits % WORD_BITS) != 0)
            valid = bv->nbits % WORD_BITS;
        uint64_t zeros_word = (~bv->words[word_index]) & valid_mask(valid);
        size_t pc = popcount64(zeros_word);
        if (remain < pc) {
            size_t bit = 0;
            if (!select_in_word(zeros_word, remain, &bit)) return false;
            *out_index = word_index * (size_t)WORD_BITS + bit;
            return *out_index < bv->nbits;
        }
        remain -= pc;
    }
    return false;
}

bool sbv_validate(const SuccinctBitVector *bv) {
    if (!bv || !bv->super_rank) return false;
    if (bv->nbits > SIZE_MAX - (WORD_BITS - 1U)) return false;
    size_t wc = (bv->nbits + (WORD_BITS - 1U)) / WORD_BITS;
    size_t sc = wc == 0 ? 0 : (wc + SUPER_WORDS - 1U) / SUPER_WORDS;
    if (wc != bv->word_count || sc != bv->super_count) return false;
    if (wc > 0 && !bv->words) return false;
    if (wc == 0 && bv->words) return false;

    if (wc && (bv->nbits % WORD_BITS) != 0) {
        size_t valid = bv->nbits % WORD_BITS;
        if ((bv->words[wc - 1U] & ~valid_mask(valid)) != 0) return false;
    }

    size_t running = 0;
    for (size_t s = 0; s < sc; ++s) {
        if (bv->super_rank[s] != running) return false;
        size_t start = s * (size_t)SUPER_WORDS;
        size_t count = words_in_super(bv, s);
        for (size_t j = 0; j < count; ++j) running += popcount64(bv->words[start + j]);
    }
    return bv->super_rank[sc] == running && bv->ones == running;
}
