#include "mpt.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
enum{N=10000};
static void key_for(size_t i,unsigned char k[MPT_KEY_BYTES]){
    for(size_t j=0;j<MPT_KEY_BYTES;++j)k[j]=(unsigned char)((i*131U+j*17U+(i>>(j%7U)))&255U);
    k[0]=(unsigned char)(i>>8U);k[1]=(unsigned char)i;k[31]^=(unsigned char)(i*29U);
}
static uint64_t rng=UINT64_C(0x123456789abcdef);
static uint64_t next_rng(void){rng^=rng<<13;rng^=rng>>7;rng^=rng<<17;return rng;}
int main(void){
    Mpt*a=mpt_create(),*b=mpt_create();assert(a&&b);
    unsigned char key[MPT_KEY_BYTES];
    for(size_t i=0;i<N;++i){key_for(i,key);bool inserted=false;assert(mpt_put(a,key,(uint64_t)i*11U,&inserted)&&inserted);}
    size_t order[N];for(size_t i=0;i<N;++i)order[i]=i;
    for(size_t i=N;i>1U;--i){size_t j=(size_t)(next_rng()%i);size_t t=order[i-1U];order[i-1U]=order[j];order[j]=t;}
    for(size_t oi=0;oi<N;++oi){size_t i=order[oi];key_for(i,key);bool inserted=false;assert(mpt_put(b,key,(uint64_t)i*11U,&inserted)&&inserted);}
    assert(mpt_size(a)==N&&mpt_size(b)==N&&mpt_validate(a)&&mpt_validate(b));
    unsigned char ra[32],rb[32];assert(mpt_root(a,ra)&&mpt_root(b,rb));assert(memcmp(ra,rb,32)==0);
    for(size_t i=0;i<N;++i){key_for(i,key);uint64_t v=0;bool found=false;assert(mpt_get(a,key,&v,&found)&&found&&v==(uint64_t)i*11U);}
    unsigned char before[32];memcpy(before,ra,32);
    for(size_t i=0;i<N;i+=10U){key_for(i,key);bool inserted=true;assert(mpt_put(a,key,(uint64_t)i*13U,&inserted)&&!inserted);}
    assert(mpt_root(a,ra)&&memcmp(before,ra,32)!=0&&mpt_validate(a));
    memset(key,0xff,sizeof(key));uint64_t v=0;bool found=true;assert(mpt_get(a,key,&v,&found)&&!found);
    printf("MPT tests passed; keys=%zu nodes=%zu\n",mpt_size(a),mpt_node_count(a));
    mpt_free(b);mpt_free(a);return 0;
}
