#include "merkle_tree.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t h[8];
    uint64_t bit_count;
    unsigned char block[64];
    size_t used;
} Sha256Ctx;

static const uint32_t K[64] = {
    UINT32_C(0x428a2f98),UINT32_C(0x71374491),UINT32_C(0xb5c0fbcf),UINT32_C(0xe9b5dba5),
    UINT32_C(0x3956c25b),UINT32_C(0x59f111f1),UINT32_C(0x923f82a4),UINT32_C(0xab1c5ed5),
    UINT32_C(0xd807aa98),UINT32_C(0x12835b01),UINT32_C(0x243185be),UINT32_C(0x550c7dc3),
    UINT32_C(0x72be5d74),UINT32_C(0x80deb1fe),UINT32_C(0x9bdc06a7),UINT32_C(0xc19bf174),
    UINT32_C(0xe49b69c1),UINT32_C(0xefbe4786),UINT32_C(0x0fc19dc6),UINT32_C(0x240ca1cc),
    UINT32_C(0x2de92c6f),UINT32_C(0x4a7484aa),UINT32_C(0x5cb0a9dc),UINT32_C(0x76f988da),
    UINT32_C(0x983e5152),UINT32_C(0xa831c66d),UINT32_C(0xb00327c8),UINT32_C(0xbf597fc7),
    UINT32_C(0xc6e00bf3),UINT32_C(0xd5a79147),UINT32_C(0x06ca6351),UINT32_C(0x14292967),
    UINT32_C(0x27b70a85),UINT32_C(0x2e1b2138),UINT32_C(0x4d2c6dfc),UINT32_C(0x53380d13),
    UINT32_C(0x650a7354),UINT32_C(0x766a0abb),UINT32_C(0x81c2c92e),UINT32_C(0x92722c85),
    UINT32_C(0xa2bfe8a1),UINT32_C(0xa81a664b),UINT32_C(0xc24b8b70),UINT32_C(0xc76c51a3),
    UINT32_C(0xd192e819),UINT32_C(0xd6990624),UINT32_C(0xf40e3585),UINT32_C(0x106aa070),
    UINT32_C(0x19a4c116),UINT32_C(0x1e376c08),UINT32_C(0x2748774c),UINT32_C(0x34b0bcb5),
    UINT32_C(0x391c0cb3),UINT32_C(0x4ed8aa4a),UINT32_C(0x5b9cca4f),UINT32_C(0x682e6ff3),
    UINT32_C(0x748f82ee),UINT32_C(0x78a5636f),UINT32_C(0x84c87814),UINT32_C(0x8cc70208),
    UINT32_C(0x90befffa),UINT32_C(0xa4506ceb),UINT32_C(0xbef9a3f7),UINT32_C(0xc67178f2)
};

static uint32_t rotr(uint32_t x,unsigned n){return(x>>n)|(x<<(32U-n));}
static uint32_t load_be32(const unsigned char*p){return((uint32_t)p[0]<<24U)|((uint32_t)p[1]<<16U)|((uint32_t)p[2]<<8U)|(uint32_t)p[3];}
static void store_be32(unsigned char*p,uint32_t x){p[0]=(unsigned char)(x>>24U);p[1]=(unsigned char)(x>>16U);p[2]=(unsigned char)(x>>8U);p[3]=(unsigned char)x;}

static void sha_transform(Sha256Ctx*c,const unsigned char block[64]){
    uint32_t w[64];
    for(size_t i=0;i<16U;++i)w[i]=load_be32(block+i*4U);
    for(size_t i=16U;i<64U;++i){
        uint32_t s0=rotr(w[i-15U],7)^rotr(w[i-15U],18)^(w[i-15U]>>3U);
        uint32_t s1=rotr(w[i-2U],17)^rotr(w[i-2U],19)^(w[i-2U]>>10U);
        w[i]=w[i-16U]+s0+w[i-7U]+s1;
    }
    uint32_t a=c->h[0],b=c->h[1],cc=c->h[2],d=c->h[3],e=c->h[4],f=c->h[5],g=c->h[6],h=c->h[7];
    for(size_t i=0;i<64U;++i){
        uint32_t S1=rotr(e,6)^rotr(e,11)^rotr(e,25);
        uint32_t ch=(e&f)^((~e)&g);
        uint32_t t1=h+S1+ch+K[i]+w[i];
        uint32_t S0=rotr(a,2)^rotr(a,13)^rotr(a,22);
        uint32_t maj=(a&b)^(a&cc)^(b&cc);
        uint32_t t2=S0+maj;
        h=g;g=f;f=e;e=d+t1;d=cc;cc=b;b=a;a=t1+t2;
    }
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;
    c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}

static void sha_init(Sha256Ctx*c){
    static const uint32_t init[8]={UINT32_C(0x6a09e667),UINT32_C(0xbb67ae85),UINT32_C(0x3c6ef372),UINT32_C(0xa54ff53a),UINT32_C(0x510e527f),UINT32_C(0x9b05688c),UINT32_C(0x1f83d9ab),UINT32_C(0x5be0cd19)};
    memcpy(c->h,init,sizeof(init));c->bit_count=0U;c->used=0U;
}

static void sha_update(Sha256Ctx*c,const void*data,size_t len){
    const unsigned char*p=data;
    while(len){
        size_t space=64U-c->used;
        size_t take=len<space?len:space;
        memcpy(c->block+c->used,p,take);c->used+=take;p+=take;len-=take;
        if(c->used==64U){sha_transform(c,c->block);c->bit_count+=UINT64_C(512);c->used=0U;}
    }
}

static void sha_final(Sha256Ctx*c,unsigned char out[32]){
    uint64_t total_bits=c->bit_count+(uint64_t)c->used*8U;
    c->block[c->used++]=0x80U;
    if(c->used>56U){
        while(c->used<64U)c->block[c->used++]=0U;
        sha_transform(c,c->block);c->used=0U;
    }
    while(c->used<56U)c->block[c->used++]=0U;
    for(unsigned i=0;i<8U;++i)c->block[63U-i]=(unsigned char)(total_bits>>(i*8U));
    sha_transform(c,c->block);
    for(size_t i=0;i<8U;++i)store_be32(out+i*4U,c->h[i]);
}

void merkle_sha256(const void*data,size_t len,unsigned char out[32]){
    Sha256Ctx c;sha_init(&c);if(len)sha_update(&c,data,len);sha_final(&c,out);
}

static void hash_leaf(const unsigned char*leaf,size_t len,unsigned char out[32]){
    Sha256Ctx c;sha_init(&c);unsigned char prefix=0U;sha_update(&c,&prefix,1U);if(len)sha_update(&c,leaf,len);sha_final(&c,out);
}
static void hash_parent(const unsigned char left[32],const unsigned char right[32],unsigned char out[32]){
    Sha256Ctx c;sha_init(&c);unsigned char prefix=1U;sha_update(&c,&prefix,1U);sha_update(&c,left,32U);sha_update(&c,right,32U);sha_final(&c,out);
}

struct MerkleTree{
    size_t leaf_count;
    size_t leaf_size;
    size_t level_count;
    size_t *counts;
    unsigned char **levels;
};

struct MerkleProof{
    size_t leaf_index;
    size_t steps;
    unsigned char *siblings;
    unsigned char *sibling_left;
};

static size_t compute_levels(size_t count){
    size_t levels=1U;
    while(count>1U){count=count/2U+count%2U;++levels;}
    return levels;
}

MerkleTree *merkle_create(const void*leaves,size_t leaf_count,size_t leaf_size){
    if(!leaves||leaf_count==0U||leaf_size==0U||leaf_count>SIZE_MAX/leaf_size)return NULL;
    size_t levels=compute_levels(leaf_count);
    if(levels>SIZE_MAX/sizeof(size_t)||levels>SIZE_MAX/sizeof(unsigned char*))return NULL;

    MerkleTree*t=calloc(1,sizeof(*t));if(!t)return NULL;
    t->counts=malloc(levels*sizeof(*t->counts));
    t->levels=calloc(levels,sizeof(*t->levels));
    if(!t->counts||!t->levels){merkle_free(t);return NULL;}
    t->leaf_count=leaf_count;t->leaf_size=leaf_size;t->level_count=levels;

    size_t count=leaf_count;
    for(size_t level=0;level<levels;++level){
        t->counts[level]=count;
        if(count>SIZE_MAX/MERKLE_HASH_SIZE){merkle_free(t);return NULL;}
        t->levels[level]=malloc(count*MERKLE_HASH_SIZE);
        if(!t->levels[level]){merkle_free(t);return NULL;}
        count=count/2U+count%2U;
    }

    const unsigned char*bytes=leaves;
    for(size_t i=0;i<leaf_count;++i)
        hash_leaf(bytes+i*leaf_size,leaf_size,t->levels[0]+i*MERKLE_HASH_SIZE);

    for(size_t level=1;level<levels;++level){
        size_t prev_count=t->counts[level-1U];
        for(size_t i=0;i<t->counts[level];++i){
            size_t left_i=i*2U;
            size_t right_i=left_i+1U<prev_count?left_i+1U:left_i;
            hash_parent(t->levels[level-1U]+left_i*MERKLE_HASH_SIZE,
                        t->levels[level-1U]+right_i*MERKLE_HASH_SIZE,
                        t->levels[level]+i*MERKLE_HASH_SIZE);
        }
    }
    return t;
}

void merkle_free(MerkleTree*t){
    if(!t)return;
    if(t->levels)for(size_t i=0;i<t->level_count;++i)free(t->levels[i]);
    free(t->levels);free(t->counts);free(t);
}

bool merkle_root(const MerkleTree*t,unsigned char out[32]){
    if(!t||!out||t->level_count==0U||t->counts[t->level_count-1U]!=1U)return false;
    memcpy(out,t->levels[t->level_count-1U],32U);return true;
}
size_t merkle_leaf_count(const MerkleTree*t){return t?t->leaf_count:0U;}
size_t merkle_level_count(const MerkleTree*t){return t?t->level_count:0U;}

MerkleProof *merkle_proof_create(const MerkleTree*t,size_t leaf_index){
    if(!t||leaf_index>=t->leaf_count)return NULL;
    MerkleProof*p=calloc(1,sizeof(*p));if(!p)return NULL;
    p->leaf_index=leaf_index;p->steps=t->level_count-1U;
    if(p->steps){
        if(p->steps>SIZE_MAX/32U){free(p);return NULL;}
        p->siblings=malloc(p->steps*32U);p->sibling_left=malloc(p->steps);
        if(!p->siblings||!p->sibling_left){merkle_proof_free(p);return NULL;}
    }
    size_t index=leaf_index;
    for(size_t level=0;level<p->steps;++level){
        size_t count=t->counts[level];
        size_t sibling=index^1U;
        if(sibling>=count)sibling=index;
        memcpy(p->siblings+level*32U,t->levels[level]+sibling*32U,32U);
        p->sibling_left[level]=(unsigned char)(sibling<index);
        index/=2U;
    }
    return p;
}

void merkle_proof_free(MerkleProof*p){if(!p)return;free(p->siblings);free(p->sibling_left);free(p);}
size_t merkle_proof_steps(const MerkleProof*p){return p?p->steps:0U;}

bool merkle_verify(const void*leaf,size_t leaf_size,const MerkleProof*p,const unsigned char root[32]){
    if(!leaf||leaf_size==0U||!p||!root)return false;
    unsigned char current[32],next[32];hash_leaf(leaf,leaf_size,current);
    for(size_t i=0;i<p->steps;++i){
        const unsigned char*sib=p->siblings+i*32U;
        if(p->sibling_left[i])hash_parent(sib,current,next);else hash_parent(current,sib,next);
        memcpy(current,next,32U);
    }
    return memcmp(current,root,32U)==0;
}

bool merkle_validate(const MerkleTree*t){
    if(!t||!t->counts||!t->levels||t->leaf_count==0U||t->leaf_size==0U||
       t->level_count!=compute_levels(t->leaf_count))return false;
    if(t->counts[0]!=t->leaf_count)return false;
    for(size_t level=0;level<t->level_count;++level)if(!t->levels[level])return false;
    for(size_t level=1;level<t->level_count;++level){
        size_t previous_count=t->counts[level-1U];
        size_t expected=previous_count/2U+previous_count%2U;
        if(t->counts[level]!=expected)return false;
        size_t prev=t->counts[level-1U];
        for(size_t i=0;i<t->counts[level];++i){
            size_t li=i*2U,ri=li+1U<prev?li+1U:li;
            unsigned char h[32];
            hash_parent(t->levels[level-1U]+li*32U,t->levels[level-1U]+ri*32U,h);
            if(memcmp(h,t->levels[level]+i*32U,32U)!=0)return false;
        }
    }
    return t->counts[t->level_count-1U]==1U;
}
