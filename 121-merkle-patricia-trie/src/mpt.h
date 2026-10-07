#ifndef MPT_H
#define MPT_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define MPT_KEY_BYTES 32U
#define MPT_HASH_BYTES 32U
typedef struct Mpt Mpt;
Mpt *mpt_create(void);
void mpt_free(Mpt *trie);
bool mpt_put(Mpt *trie,const unsigned char key[MPT_KEY_BYTES],uint64_t value,bool *out_inserted);
bool mpt_get(const Mpt *trie,const unsigned char key[MPT_KEY_BYTES],uint64_t *out_value,bool *out_found);
bool mpt_root(const Mpt *trie,unsigned char out[MPT_HASH_BYTES]);
size_t mpt_size(const Mpt *trie);
size_t mpt_node_count(const Mpt *trie);
bool mpt_validate(const Mpt *trie);
#endif
