#ifndef BYTE_TRIE_H
#define BYTE_TRIE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ByteTrie ByteTrie;

typedef bool (*byte_trie_visit_fn)(
    const unsigned char *bytes,
    size_t length,
    void *context
);

ByteTrie *byte_trie_create(void);
void byte_trie_free(ByteTrie *trie);

size_t byte_trie_size(const ByteTrie *trie);

bool byte_trie_insert(ByteTrie *trie, const unsigned char *bytes, size_t length);
bool byte_trie_contains(const ByteTrie *trie, const unsigned char *bytes, size_t length);
bool byte_trie_has_prefix(const ByteTrie *trie, const unsigned char *bytes, size_t length);
size_t byte_trie_count_prefix(const ByteTrie *trie, const unsigned char *bytes, size_t length);
bool byte_trie_remove(ByteTrie *trie, const unsigned char *bytes, size_t length);

bool byte_trie_visit_prefix(
    const ByteTrie *trie,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_trie_visit_fn visitor,
    void *context
);

bool byte_trie_validate(const ByteTrie *trie);

#endif
