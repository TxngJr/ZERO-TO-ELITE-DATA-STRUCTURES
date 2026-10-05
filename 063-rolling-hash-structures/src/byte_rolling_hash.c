#include "byte_rolling_hash.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const uint64_t MOD1=UINT64_C(1000000007);
static const uint64_t MOD2=UINT64_C(1000000009);
static const uint64_t BASE=UINT64_C(911382323);

struct ByteRollingHash {
    uint8_t *text;
    size_t length;

    uint64_t *prefix1;
    uint64_t *prefix2;
    uint64_t *power1;
    uint64_t *power2;
};

static uint64_t mul_mod(
    uint64_t a,
    uint64_t b,
    uint64_t mod
) {
    return (a*b)%mod;
}

static uint64_t sub_mod(
    uint64_t a,
    uint64_t b,
    uint64_t mod
) {
    return a>=b?a-b:mod-(b-a);
}

static bool alloc_u64(
    size_t count,
    uint64_t **out
) {
    if(count>SIZE_MAX/sizeof **out)return false;

    *out=malloc(count*sizeof **out);
    return *out!=NULL;
}

ByteRollingHash *byte_rolling_hash_create(
    const uint8_t *text,
    size_t length
) {
    if(length>0&&text==NULL)return NULL;
    if(length==SIZE_MAX)return NULL;

    ByteRollingHash *hash=calloc(1,sizeof *hash);
    if(hash==NULL)return NULL;

    hash->length=length;

    if(length>0) {
        hash->text=malloc(length);

        if(hash->text==NULL) {
            byte_rolling_hash_free(hash);
            return NULL;
        }

        memcpy(hash->text,text,length);
    }

    const size_t count=length+1;

    if(!alloc_u64(count,&hash->prefix1)||
       !alloc_u64(count,&hash->prefix2)||
       !alloc_u64(count,&hash->power1)||
       !alloc_u64(count,&hash->power2)) {
        byte_rolling_hash_free(hash);
        return NULL;
    }

    hash->prefix1[0]=0;
    hash->prefix2[0]=0;
    hash->power1[0]=1;
    hash->power2[0]=1;

    for(size_t i=0;i<length;++i) {
        const uint64_t value=(uint64_t)hash->text[i]+1;

        hash->prefix1[i+1]=(
            mul_mod(hash->prefix1[i],BASE,MOD1)+value
        )%MOD1;

        hash->prefix2[i+1]=(
            mul_mod(hash->prefix2[i],BASE,MOD2)+value
        )%MOD2;

        hash->power1[i+1]=mul_mod(
            hash->power1[i],BASE,MOD1
        );

        hash->power2[i+1]=mul_mod(
            hash->power2[i],BASE,MOD2
        );
    }

    return hash;
}

void byte_rolling_hash_free(ByteRollingHash *hash) {
    if(hash==NULL)return;

    free(hash->text);
    free(hash->prefix1);
    free(hash->prefix2);
    free(hash->power1);
    free(hash->power2);
    free(hash);
}

size_t byte_rolling_hash_size(const ByteRollingHash *hash) {
    return hash==NULL?0:hash->length;
}

static ByteHashPair range_unchecked(
    const ByteRollingHash *hash,
    size_t left,
    size_t right
) {
    const size_t len=right-left;

    const uint64_t remove1=mul_mod(
        hash->prefix1[left],
        hash->power1[len],
        MOD1
    );

    const uint64_t remove2=mul_mod(
        hash->prefix2[left],
        hash->power2[len],
        MOD2
    );

    return (ByteHashPair){
        .first=sub_mod(hash->prefix1[right],remove1,MOD1),
        .second=sub_mod(hash->prefix2[right],remove2,MOD2)
    };
}

bool byte_rolling_hash_range(
    const ByteRollingHash *hash,
    size_t left,
    size_t right,
    ByteHashPair *out_hash
) {
    if(hash==NULL||out_hash==NULL||
       left>right||right>hash->length) {
        return false;
    }

    *out_hash=range_unchecked(hash,left,right);
    return true;
}

bool byte_rolling_hash_equal(
    const ByteRollingHash *hash,
    size_t left_a,
    size_t right_a,
    size_t left_b,
    size_t right_b,
    bool *out_equal
) {
    if(hash==NULL||out_equal==NULL||
       left_a>right_a||right_a>hash->length||
       left_b>right_b||right_b>hash->length) {
        return false;
    }

    const size_t len_a=right_a-left_a;
    const size_t len_b=right_b-left_b;

    if(len_a!=len_b) {
        *out_equal=false;
        return true;
    }

    const ByteHashPair a=range_unchecked(
        hash,left_a,right_a
    );

    const ByteHashPair b=range_unchecked(
        hash,left_b,right_b
    );

    if(a.first!=b.first||a.second!=b.second) {
        *out_equal=false;
        return true;
    }

    *out_equal=len_a==0||
        memcmp(
            hash->text+left_a,
            hash->text+left_b,
            len_a
        )==0;

    return true;
}

bool byte_rolling_hash_lcp(
    const ByteRollingHash *hash,
    size_t start_a,
    size_t start_b,
    size_t max_length,
    size_t *out_lcp
) {
    if(hash==NULL||out_lcp==NULL||
       start_a>hash->length||
       start_b>hash->length||
       max_length>hash->length-start_a||
       max_length>hash->length-start_b) {
        return false;
    }

    size_t low=0;
    size_t high=max_length+1;

    while(low+1<high) {
        const size_t mid=low+(high-low)/2;

        const ByteHashPair a=range_unchecked(
            hash,start_a,start_a+mid
        );
        const ByteHashPair b=range_unchecked(
            hash,start_b,start_b+mid
        );

        if(a.first==b.first&&a.second==b.second) {
            low=mid;
        } else {
            high=mid;
        }
    }

    size_t exact=0;

    while(exact<max_length&&
          hash->text[start_a+exact]==
              hash->text[start_b+exact]) {
        ++exact;
    }

    /*
     * Exact verification dominates a theoretical collision.
     * Under no collision exact==low; otherwise exact is authoritative.
     */
    *out_lcp=exact;
    return true;
}

static ByteHashPair hash_pattern(
    const uint8_t *pattern,
    size_t length
) {
    uint64_t h1=0;
    uint64_t h2=0;

    for(size_t i=0;i<length;++i) {
        const uint64_t value=(uint64_t)pattern[i]+1;

        h1=(mul_mod(h1,BASE,MOD1)+value)%MOD1;
        h2=(mul_mod(h2,BASE,MOD2)+value)%MOD2;
    }

    return (ByteHashPair){
        .first=h1,
        .second=h2
    };
}

static bool scan_pattern(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_count,
    bool report
) {
    if(hash==NULL||pattern==NULL||
       pattern_length==0||out_count==NULL) {
        return false;
    }

    if(pattern_length>hash->length) {
        *out_count=0;
        return true;
    }

    const ByteHashPair expected=
        hash_pattern(pattern,pattern_length);

    size_t count=0;

    for(size_t i=0;
        i+pattern_length<=hash->length;
        ++i) {
        const ByteHashPair actual=range_unchecked(
            hash,i,i+pattern_length
        );

        if(actual.first!=expected.first||
           actual.second!=expected.second) {
            continue;
        }

        if(memcmp(
                hash->text+i,
                pattern,
                pattern_length
            )!=0) {
            continue;
        }

        if(report) {
            if(count>=capacity)return false;
            positions[count]=i;
        }

        ++count;
    }

    *out_count=count;
    return true;
}

bool byte_rolling_hash_contains(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length
) {
    size_t count=0;

    return scan_pattern(
        hash,pattern,pattern_length,
        NULL,0,&count,false
    )&&count>0;
}

bool byte_rolling_hash_count(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *out_count
) {
    return scan_pattern(
        hash,pattern,pattern_length,
        NULL,0,out_count,false
    );
}

bool byte_rolling_hash_report(
    const ByteRollingHash *hash,
    const uint8_t *pattern,
    size_t pattern_length,
    size_t *positions,
    size_t capacity,
    size_t *out_written
) {
    if(out_written==NULL)return false;

    size_t needed=0;

    if(!byte_rolling_hash_count(
            hash,pattern,pattern_length,&needed
        )) {
        return false;
    }

    if(needed>capacity||(needed>0&&positions==NULL)) {
        return false;
    }

    return scan_pattern(
        hash,pattern,pattern_length,
        positions,capacity,out_written,true
    );
}

bool byte_rolling_hash_validate(const ByteRollingHash *hash) {
    if(hash==NULL||
       hash->prefix1==NULL||
       hash->prefix2==NULL||
       hash->power1==NULL||
       hash->power2==NULL||
       (hash->length>0&&hash->text==NULL)) {
        return false;
    }

    if(hash->prefix1[0]!=0||
       hash->prefix2[0]!=0||
       hash->power1[0]!=1||
       hash->power2[0]!=1) {
        return false;
    }

    for(size_t i=0;i<hash->length;++i) {
        const uint64_t value=(uint64_t)hash->text[i]+1;

        const uint64_t p1=(
            mul_mod(hash->prefix1[i],BASE,MOD1)+value
        )%MOD1;

        const uint64_t p2=(
            mul_mod(hash->prefix2[i],BASE,MOD2)+value
        )%MOD2;

        const uint64_t pow1=mul_mod(
            hash->power1[i],BASE,MOD1
        );

        const uint64_t pow2=mul_mod(
            hash->power2[i],BASE,MOD2
        );

        if(hash->prefix1[i+1]!=p1||
           hash->prefix2[i+1]!=p2||
           hash->power1[i+1]!=pow1||
           hash->power2[i+1]!=pow2) {
            return false;
        }
    }

    return true;
}
