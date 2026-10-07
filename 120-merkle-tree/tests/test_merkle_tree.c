#include "merkle_tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum{LEAVES=4097,SIZE=32};
static const unsigned char ABC_SHA[32]={0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
int main(void){
    unsigned char sha[32];merkle_sha256("abc",3,sha);assert(memcmp(sha,ABC_SHA,32)==0);
    unsigned char*data=malloc((size_t)LEAVES*SIZE);assert(data);
    for(size_t i=0;i<LEAVES;++i)for(size_t j=0;j<SIZE;++j)data[i*SIZE+j]=(unsigned char)((i*31U+j*17U)&255U);
    MerkleTree*t=merkle_create(data,LEAVES,SIZE);assert(t&&merkle_validate(t));unsigned char root[32];assert(merkle_root(t,root));
    for(size_t i=0;i<LEAVES;i+=37U){MerkleProof*p=merkle_proof_create(t,i);assert(p);assert(merkle_proof_steps(p)==merkle_level_count(t)-1U);assert(merkle_verify(data+i*SIZE,SIZE,p,root));unsigned char tmp[SIZE];memcpy(tmp,data+i*SIZE,SIZE);tmp[0]^=1U;assert(!merkle_verify(tmp,SIZE,p,root));merkle_proof_free(p);}
    unsigned char old=data[123U*SIZE];data[123U*SIZE]^=1U;MerkleTree*t2=merkle_create(data,LEAVES,SIZE);assert(t2);unsigned char root2[32];assert(merkle_root(t2,root2));assert(memcmp(root,root2,32)!=0);data[123U*SIZE]=old;merkle_free(t2);
    unsigned char one[3]={1,2,3};MerkleTree*s=merkle_create(one,1,3);assert(s);MerkleProof*p=merkle_proof_create(s,0);assert(p&&merkle_proof_steps(p)==0U);unsigned char sr[32];assert(merkle_root(s,sr)&&merkle_verify(one,3,p,sr));merkle_proof_free(p);merkle_free(s);
    printf("Merkle tests passed; leaves=%zu levels=%zu\n",merkle_leaf_count(t),merkle_level_count(t));
    merkle_free(t);free(data);return 0;
}
