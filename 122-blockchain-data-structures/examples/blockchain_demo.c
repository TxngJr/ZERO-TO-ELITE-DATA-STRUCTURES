#include "blockchain.h"
#include <stdio.h>
int main(void){Blockchain*c=bc_create();if(!c)return 1;BcTransaction tx[2]={{1,2,50,0},{2,3,10,0}};if(!bc_append(c,tx,2,1700000000))return 2;BcBlockInfo info;if(!bc_block_info(c,0,&info))return 3;printf("height=%llu txs=%zu hash=%02x%02x%02x%02x...\n",(unsigned long long)info.height,info.tx_count,info.hash[0],info.hash[1],info.hash[2],info.hash[3]);bc_free(c);return 0;}
