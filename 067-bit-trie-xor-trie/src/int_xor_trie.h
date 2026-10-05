#ifndef INT_XOR_TRIE_H
#define INT_XOR_TRIE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct IntXorTrie IntXorTrie;

IntXorTrie *int_xor_trie_create(void);
void int_xor_trie_free(IntXorTrie *trie);

size_t int_xor_trie_size(const IntXorTrie *trie);
size_t int_xor_trie_node_count(const IntXorTrie *trie);

bool int_xor_trie_insert(IntXorTrie *trie,uint64_t value);
bool int_xor_trie_remove(IntXorTrie *trie,uint64_t value);

bool int_xor_trie_contains(
    const IntXorTrie *trie,
    uint64_t value,
    bool *out_present
);

bool int_xor_trie_min_xor(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t *out_value,
    uint64_t *out_xor
);

bool int_xor_trie_max_xor(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t *out_value,
    uint64_t *out_xor
);

bool int_xor_trie_count_xor_less_than(
    const IntXorTrie *trie,
    uint64_t query,
    uint64_t limit,
    size_t *out_count
);

bool int_xor_trie_validate(const IntXorTrie *trie);

#endif
