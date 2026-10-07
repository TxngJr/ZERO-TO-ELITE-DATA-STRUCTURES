#include "merkle_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
enum{N=100000,S=32,VERIFY=10000};static double e(struct timespec a,struct timespec b){return(double)(b.tv_sec-a.tv_sec)+(double)(b.tv_nsec-a.tv_nsec)/1e9;}
int main(void){unsigned char*data=malloc((size_t)N*S);if(!data)return 1;for(size_t i=0;i<(size_t)N*S;++i)data[i]=(unsigned char)(i*37U);struct timespec a,b,c;timespec_get(&a,TIME_UTC);MerkleTree*t=merkle_create(data,N,S);if(!t)return 2;timespec_get(&b,TIME_UTC);unsigned char root[32];merkle_root(t,root);for(size_t n=0;n<VERIFY;++n){size_t i=(n*7919U)%N;MerkleProof*p=merkle_proof_create(t,i);if(!p||!merkle_verify(data+i*S,S,p,root))return 3;merkle_proof_free(p);}timespec_get(&c,TIME_UTC);printf("leaves=%d build_seconds=%.6f verifies=%d verify_seconds=%.6f root_prefix=%02x%02x%02x%02x\n",N,e(a,b),VERIFY,e(b,c),root[0],root[1],root[2],root[3]);merkle_free(t);free(data);return 0;}
