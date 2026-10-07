#include "merkle_tree.h"
#include <stdio.h>
int main(void){const char leaves[4][4]={"one","two","tre","for"};MerkleTree*t=merkle_create(leaves,4,4);if(!t)return 1;unsigned char root[32];if(!merkle_root(t,root))return 2;for(size_t i=0;i<8;++i)printf("%02x",root[i]);puts("...");MerkleProof*p=merkle_proof_create(t,2);if(!p||!merkle_verify(leaves[2],4,p,root))return 3;merkle_proof_free(p);merkle_free(t);return 0;}
