#include "blockchain.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
enum{BLOCKS=200,TXS=64};
static void fill(size_t block,BcTransaction tx[TXS]){
    for(size_t i=0;i<TXS;++i){
        tx[i].from=1000U+block*TXS+i;
        tx[i].to=2000U+((i*17U+block)%10000U);
        tx[i].amount=1U+((block+1U)*(i+3U))%100000U;
        tx[i].nonce=block*TXS+i;
    }
}
int main(void){
    Blockchain*c=bc_create();assert(c);BcTransaction tx[TXS];
    for(size_t b=0;b<BLOCKS;++b){
        fill(b,tx);assert(bc_append(c,tx,TXS,UINT64_C(1700000000)+b));
        tx[0].amount^=UINT64_C(0xffff);
        BcTransaction stored;assert(bc_transaction(c,b,0,&stored));
        assert(stored.amount!=(tx[0].amount));
    }
    assert(bc_block_count(c)==BLOCKS&&bc_validate(c));
    unsigned char prev[32]={0};
    for(size_t b=0;b<BLOCKS;++b){
        BcBlockInfo info;assert(bc_block_info(c,b,&info));
        assert(info.height==b&&info.tx_count==TXS);
        assert(memcmp(info.prev_hash,prev,32U)==0);
        memcpy(prev,info.hash,32U);
        if((b%17U)==0U){
            size_t ti=(b*13U)%TXS;BcTransaction stored;
            assert(bc_transaction(c,b,ti,&stored));
            BcTxProof*p=bc_tx_proof_create(c,b,ti);assert(p);
            assert(bc_tx_proof_verify(&stored,p,info.tx_root));
            stored.amount++;
            assert(!bc_tx_proof_verify(&stored,p,info.tx_root));
            bc_tx_proof_free(p);
        }
    }
    puts("Blockchain tests passed");
    bc_free(c);return 0;
}
