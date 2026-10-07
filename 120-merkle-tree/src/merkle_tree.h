#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H
#include <stdbool.h>
#include <stddef.h>
#define MERKLE_HASH_SIZE 32U
typedef struct MerkleTree MerkleTree;
typedef struct MerkleProof MerkleProof;
void merkle_sha256(const void *data,size_t len,unsigned char out[MERKLE_HASH_SIZE]);
MerkleTree *merkle_create(const void *leaves,size_t leaf_count,size_t leaf_size);
void merkle_free(MerkleTree *tree);
bool merkle_root(const MerkleTree *tree,unsigned char out[MERKLE_HASH_SIZE]);
size_t merkle_leaf_count(const MerkleTree *tree);
size_t merkle_level_count(const MerkleTree *tree);
MerkleProof *merkle_proof_create(const MerkleTree *tree,size_t leaf_index);
void merkle_proof_free(MerkleProof *proof);
size_t merkle_proof_steps(const MerkleProof *proof);
bool merkle_verify(const void *leaf,size_t leaf_size,const MerkleProof *proof,const unsigned char root[MERKLE_HASH_SIZE]);
bool merkle_validate(const MerkleTree *tree);
#endif
