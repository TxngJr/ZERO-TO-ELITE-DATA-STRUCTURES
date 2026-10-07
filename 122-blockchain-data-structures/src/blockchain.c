#include "blockchain.h"
#include "merkle_tree.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    BcBlockInfo info;
    BcTransaction *transactions;
} BcBlock;

struct Blockchain {
    BcBlock *blocks;
    size_t count;
    size_t capacity;
};

struct BcTxProof {
    size_t block_index;
    size_t tx_index;
    MerkleProof *proof;
};

static void store_be64(unsigned char out[8],uint64_t value){
    for(unsigned i=0;i<8U;++i)
        out[7U-i]=(unsigned char)(value>>(i*8U));
}

static void serialize_tx(const BcTransaction *tx,unsigned char out[32]){
    store_be64(out,tx->from);
    store_be64(out+8U,tx->to);
    store_be64(out+16U,tx->amount);
    store_be64(out+24U,tx->nonce);
}

static bool build_tx_tree(const BcTransaction *txs,size_t count,
                          MerkleTree **out_tree){
    if(!txs||count==0U||!out_tree||count>SIZE_MAX/32U)return false;
    unsigned char *records=malloc(count*32U);
    if(!records)return false;
    for(size_t i=0;i<count;++i)serialize_tx(&txs[i],records+i*32U);
    MerkleTree *tree=merkle_create(records,count,32U);
    free(records);
    if(!tree)return false;
    *out_tree=tree;
    return true;
}

static void block_hash(const BcBlockInfo *info,unsigned char out[32]){
    unsigned char buf[1U+8U+32U+32U+8U+8U];
    size_t p=0U;
    buf[p++]=0x30U;
    store_be64(buf+p,info->height);p+=8U;
    memcpy(buf+p,info->prev_hash,32U);p+=32U;
    memcpy(buf+p,info->tx_root,32U);p+=32U;
    store_be64(buf+p,info->timestamp);p+=8U;
    store_be64(buf+p,(uint64_t)info->tx_count);p+=8U;
    merkle_sha256(buf,p,out);
}

Blockchain *bc_create(void){return calloc(1,sizeof(Blockchain));}

void bc_free(Blockchain *chain){
    if(!chain)return;
    for(size_t i=0;i<chain->count;++i)
        free(chain->blocks[i].transactions);
    free(chain->blocks);
    free(chain);
}

static bool grow(Blockchain *chain){
    if(chain->capacity>SIZE_MAX/2U)return false;
    size_t new_capacity=chain->capacity?chain->capacity*2U:8U;
    if(new_capacity>SIZE_MAX/sizeof(BcBlock))return false;
    BcBlock *grown=realloc(chain->blocks,new_capacity*sizeof(*grown));
    if(!grown)return false;
    chain->blocks=grown;
    chain->capacity=new_capacity;
    return true;
}

bool bc_append(Blockchain *chain,const BcTransaction *transactions,
               size_t tx_count,uint64_t timestamp){
    if(!chain||!transactions||tx_count==0U||
       tx_count>SIZE_MAX/sizeof(BcTransaction))return false;
    if(chain->count==chain->capacity&&!grow(chain))return false;

    BcTransaction *copy=malloc(tx_count*sizeof(*copy));
    if(!copy)return false;
    memcpy(copy,transactions,tx_count*sizeof(*copy));

    MerkleTree *tree=NULL;
    if(!build_tx_tree(copy,tx_count,&tree)){free(copy);return false;}

    BcBlock block;
    memset(&block,0,sizeof(block));
    block.transactions=copy;
    block.info.height=(uint64_t)chain->count;
    block.info.timestamp=timestamp;
    block.info.tx_count=tx_count;
    if(chain->count)
        memcpy(block.info.prev_hash,chain->blocks[chain->count-1U].info.hash,32U);
    if(!merkle_root(tree,block.info.tx_root)){
        merkle_free(tree);free(copy);return false;
    }
    block_hash(&block.info,block.info.hash);
    merkle_free(tree);

    chain->blocks[chain->count++]=block;
    return true;
}

size_t bc_block_count(const Blockchain *chain){return chain?chain->count:0U;}

bool bc_block_info(const Blockchain *chain,size_t block_index,BcBlockInfo *out){
    if(!chain||!out||block_index>=chain->count)return false;
    *out=chain->blocks[block_index].info;
    return true;
}

bool bc_transaction(const Blockchain *chain,size_t block_index,size_t tx_index,
                    BcTransaction *out){
    if(!chain||!out||block_index>=chain->count)return false;
    const BcBlock *block=&chain->blocks[block_index];
    if(tx_index>=block->info.tx_count)return false;
    *out=block->transactions[tx_index];
    return true;
}

BcTxProof *bc_tx_proof_create(const Blockchain *chain,size_t block_index,
                              size_t tx_index){
    if(!chain||block_index>=chain->count)return NULL;
    const BcBlock *block=&chain->blocks[block_index];
    if(tx_index>=block->info.tx_count)return NULL;

    MerkleTree *tree=NULL;
    if(!build_tx_tree(block->transactions,block->info.tx_count,&tree))return NULL;
    MerkleProof *inner=merkle_proof_create(tree,tx_index);
    merkle_free(tree);
    if(!inner)return NULL;

    BcTxProof *proof=malloc(sizeof(*proof));
    if(!proof){merkle_proof_free(inner);return NULL;}
    proof->block_index=block_index;
    proof->tx_index=tx_index;
    proof->proof=inner;
    return proof;
}

void bc_tx_proof_free(BcTxProof *proof){
    if(!proof)return;
    merkle_proof_free(proof->proof);
    free(proof);
}

bool bc_tx_proof_verify(const BcTransaction *tx,const BcTxProof *proof,
                        const unsigned char tx_root[32]){
    if(!tx||!proof||!proof->proof||!tx_root)return false;
    unsigned char record[32];
    serialize_tx(tx,record);
    return merkle_verify(record,sizeof(record),proof->proof,tx_root);
}

bool bc_validate(const Blockchain *chain){
    if(!chain)return false;
    unsigned char zero[32]={0};
    for(size_t i=0;i<chain->count;++i){
        const BcBlock *block=&chain->blocks[i];
        if(block->info.height!=(uint64_t)i||block->info.tx_count==0U||
           !block->transactions)return false;

        if(i==0U){
            if(memcmp(block->info.prev_hash,zero,32U)!=0)return false;
        }else if(memcmp(block->info.prev_hash,
                       chain->blocks[i-1U].info.hash,32U)!=0)return false;

        MerkleTree *tree=NULL;
        if(!build_tx_tree(block->transactions,block->info.tx_count,&tree))
            return false;
        unsigned char root[32];
        bool root_ok=merkle_root(tree,root)&&
                     memcmp(root,block->info.tx_root,32U)==0;
        merkle_free(tree);
        if(!root_ok)return false;

        unsigned char expected[32];
        block_hash(&block->info,expected);
        if(memcmp(expected,block->info.hash,32U)!=0)return false;
    }
    return true;
}
