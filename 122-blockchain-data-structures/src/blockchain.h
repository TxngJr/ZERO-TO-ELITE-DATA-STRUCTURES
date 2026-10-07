#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define BC_HASH_BYTES 32U
typedef struct {
    uint64_t from;
    uint64_t to;
    uint64_t amount;
    uint64_t nonce;
} BcTransaction;
typedef struct Blockchain Blockchain;
typedef struct BcTxProof BcTxProof;
typedef struct {
    uint64_t height;
    uint64_t timestamp;
    size_t tx_count;
    unsigned char prev_hash[BC_HASH_BYTES];
    unsigned char tx_root[BC_HASH_BYTES];
    unsigned char hash[BC_HASH_BYTES];
} BcBlockInfo;
Blockchain *bc_create(void);
void bc_free(Blockchain *chain);
bool bc_append(Blockchain *chain,const BcTransaction *transactions,size_t tx_count,uint64_t timestamp);
size_t bc_block_count(const Blockchain *chain);
bool bc_block_info(const Blockchain *chain,size_t block_index,BcBlockInfo *out_info);
bool bc_transaction(const Blockchain *chain,size_t block_index,size_t tx_index,BcTransaction *out_tx);
BcTxProof *bc_tx_proof_create(const Blockchain *chain,size_t block_index,size_t tx_index);
void bc_tx_proof_free(BcTxProof *proof);
bool bc_tx_proof_verify(const BcTransaction *transaction,const BcTxProof *proof,const unsigned char tx_root[BC_HASH_BYTES]);
bool bc_validate(const Blockchain *chain);
#endif
