#include "blockchain.h"
#include <stdio.h>
#include <time.h>
enum{BLOCKS=1000,TXS=32,PROOFS=1000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){Blockchain*c=bc_create();if(!c)return 1;BcTransaction tx[TXS];struct timespec a,b,d;timespec_get(&a,TIME_UTC);for(size_t bi=0;bi<BLOCKS;++bi){for(size_t i=0;i<TXS;++i)tx[i]=(BcTransaction){bi*TXS+i,bi*TXS+i+1U,(i+1U)*7U,bi*TXS+i};if(!bc_append(c,tx,TXS,1700000000U+bi))return 2;}timespec_get(&b,TIME_UTC);if(!bc_validate(c))return 3;for(size_t n=0;n<PROOFS;++n){size_t bi=(n*37U)%BLOCKS,ti=(n*13U)%TXS;BcBlockInfo info;BcTransaction t;if(!bc_block_info(c,bi,&info)||!bc_transaction(c,bi,ti,&t))return 4;BcTxProof*p=bc_tx_proof_create(c,bi,ti);if(!p||!bc_tx_proof_verify(&t,p,info.tx_root))return 5;bc_tx_proof_free(p);}timespec_get(&d,TIME_UTC);printf("blocks=%d append_seconds=%.6f validate_proof_seconds=%.6f\n",BLOCKS,e(a,b),e(b,d));bc_free(c);return 0;}
