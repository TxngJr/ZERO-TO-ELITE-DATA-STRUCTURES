#include "atomic_structures.h"
#include <stdatomic.h>
#include <stdlib.h>

struct AtomicBitset {
    size_t nbits;
    size_t word_count;
    _Atomic uint64_t *words;
};

struct AtomicTaggedValue {
    _Atomic uint64_t state;
};

static uint64_t valid_mask(size_t bits) {
    if (bits >= 64U) return UINT64_MAX;
    if (bits == 0U) return UINT64_C(0);
    return (UINT64_C(1) << bits) - UINT64_C(1);
}

static size_t popcount64(uint64_t x) {
    size_t c = 0;
    while (x) {
        x &= x - 1U;
        ++c;
    }
    return c;
}

AtomicBitset *abs_create(size_t nbits) {
    if (nbits > SIZE_MAX - 63U) return NULL;
    size_t words = (nbits + 63U) / 64U;
    if (words > SIZE_MAX / sizeof(_Atomic uint64_t)) return NULL;

    AtomicBitset *set = calloc(1, sizeof(*set));
    if (!set) return NULL;

    set->nbits = nbits;
    set->word_count = words;
    if (words) {
        set->words = malloc(words * sizeof(*set->words));
        if (!set->words) {
            free(set);
            return NULL;
        }
        for (size_t i = 0; i < words; ++i)
            atomic_init(&set->words[i], UINT64_C(0));
    }
    return set;
}

void abs_free(AtomicBitset *set) {
    if (!set) return;
    free(set->words);
    free(set);
}

size_t abs_size(const AtomicBitset *set) {
    return set ? set->nbits : 0U;
}

bool abs_platform_lock_free(const AtomicBitset *set) {
    if (!set) return false;
    for (size_t i = 0; i < set->word_count; ++i) {
        if (!atomic_is_lock_free(&set->words[i])) return false;
    }
    return true;
}

bool abs_test(const AtomicBitset *set, size_t index, bool *out_value) {
    if (!set || !out_value || index >= set->nbits) return false;
    uint64_t word =
        atomic_load_explicit(&set->words[index / 64U], memory_order_acquire);
    *out_value = ((word >> (index % 64U)) & UINT64_C(1)) != 0;
    return true;
}

bool abs_test_and_set(AtomicBitset *set, size_t index, bool *out_previous) {
    if (!set || index >= set->nbits) return false;
    uint64_t mask = UINT64_C(1) << (index % 64U);
    uint64_t old = atomic_fetch_or_explicit(
        &set->words[index / 64U], mask, memory_order_acq_rel);
    if (out_previous) *out_previous = (old & mask) != 0;
    return true;
}

bool abs_test_and_clear(AtomicBitset *set, size_t index, bool *out_previous) {
    if (!set || index >= set->nbits) return false;
    uint64_t mask = UINT64_C(1) << (index % 64U);
    uint64_t old = atomic_fetch_and_explicit(
        &set->words[index / 64U], ~mask, memory_order_acq_rel);
    if (out_previous) *out_previous = (old & mask) != 0;
    return true;
}

bool abs_test_and_toggle(AtomicBitset *set, size_t index, bool *out_previous) {
    if (!set || index >= set->nbits) return false;
    uint64_t mask = UINT64_C(1) << (index % 64U);
    uint64_t old = atomic_fetch_xor_explicit(
        &set->words[index / 64U], mask, memory_order_acq_rel);
    if (out_previous) *out_previous = (old & mask) != 0;
    return true;
}

bool abs_compare_exchange_word(AtomicBitset *set, size_t word_index,
                               uint64_t *expected, uint64_t desired,
                               bool *out_swapped) {
    if (!set || !expected || !out_swapped || word_index >= set->word_count)
        return false;

    if (word_index + 1U == set->word_count &&
        (set->nbits % 64U) != 0U) {
        uint64_t mask = valid_mask(set->nbits % 64U);
        if ((desired & ~mask) != 0U) return false;
    }

    bool swapped = atomic_compare_exchange_strong_explicit(
        &set->words[word_index], expected, desired,
        memory_order_acq_rel, memory_order_acquire);
    *out_swapped = swapped;
    return true;
}

size_t abs_count_quiescent(const AtomicBitset *set) {
    if (!set) return 0U;
    size_t total = 0U;

    for (size_t i = 0; i < set->word_count; ++i) {
        uint64_t word =
            atomic_load_explicit(&set->words[i], memory_order_acquire);
        if (i + 1U == set->word_count && (set->nbits % 64U) != 0U)
            word &= valid_mask(set->nbits % 64U);
        total += popcount64(word);
    }
    return total;
}

bool abs_validate_quiescent(const AtomicBitset *set) {
    if (!set || set->nbits > SIZE_MAX - 63U) return false;

    size_t words = (set->nbits + 63U) / 64U;
    if (words != set->word_count) return false;
    if ((words == 0U) != (set->words == NULL)) return false;

    if (words && (set->nbits % 64U) != 0U) {
        uint64_t last =
            atomic_load_explicit(&set->words[words - 1U], memory_order_acquire);
        if ((last & ~valid_mask(set->nbits % 64U)) != 0U) return false;
    }
    return true;
}

static uint64_t pack_state(uint32_t value, uint32_t version) {
    return ((uint64_t)version << 32U) | (uint64_t)value;
}

static ATVSnapshot unpack_state(uint64_t state) {
    ATVSnapshot snapshot = {
        (uint32_t)state,
        (uint32_t)(state >> 32U)
    };
    return snapshot;
}

AtomicTaggedValue *atv_create(uint32_t initial_value) {
    AtomicTaggedValue *value = malloc(sizeof(*value));
    if (!value) return NULL;
    atomic_init(&value->state, pack_state(initial_value, 0U));
    return value;
}

void atv_free(AtomicTaggedValue *value) {
    free(value);
}

bool atv_platform_lock_free(const AtomicTaggedValue *value) {
    return value && atomic_is_lock_free(&value->state);
}

bool atv_load(const AtomicTaggedValue *value, ATVSnapshot *out) {
    if (!value || !out) return false;
    *out = unpack_state(
        atomic_load_explicit(&value->state, memory_order_acquire));
    return true;
}

bool atv_compare_exchange(AtomicTaggedValue *value, ATVSnapshot *expected,
                          uint32_t desired_value, bool *out_swapped) {
    if (!value || !expected || !out_swapped ||
        expected->version == UINT32_MAX)
        return false;

    uint64_t exp = pack_state(expected->value, expected->version);
    uint64_t desired =
        pack_state(desired_value, expected->version + 1U);

    bool swapped = atomic_compare_exchange_strong_explicit(
        &value->state, &exp, desired,
        memory_order_acq_rel, memory_order_acquire);

    if (!swapped) *expected = unpack_state(exp);
    *out_swapped = swapped;
    return true;
}

bool atv_fetch_increment(AtomicTaggedValue *value, uint32_t *out_previous,
                         size_t *out_retries) {
    if (!value) return false;

    uint64_t observed =
        atomic_load_explicit(&value->state, memory_order_acquire);
    size_t retries = 0U;

    for (;;) {
        ATVSnapshot current = unpack_state(observed);
        if (current.version == UINT32_MAX ||
            current.value == UINT32_MAX)
            return false;

        uint64_t desired =
            pack_state(current.value + 1U, current.version + 1U);

        if (atomic_compare_exchange_weak_explicit(
                &value->state, &observed, desired,
                memory_order_acq_rel, memory_order_acquire)) {
            if (out_previous) *out_previous = current.value;
            if (out_retries) *out_retries = retries;
            return true;
        }
        ++retries;
    }
}
