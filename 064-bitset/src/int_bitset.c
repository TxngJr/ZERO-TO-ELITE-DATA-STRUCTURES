#include "int_bitset.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct IntBitset {
    size_t bit_count;
    size_t word_count;
    uint64_t *words;
};

static size_t words_for_bits(size_t bits) {
    return bits/64U+(bits%64U!=0U?1U:0U);
}

static uint64_t final_mask(const IntBitset *set) {
    if(set->bit_count==0)return UINT64_C(0);

    const unsigned used=(unsigned)(set->bit_count%64U);

    if(used==0U)return UINT64_MAX;

    return (UINT64_C(1)<<used)-UINT64_C(1);
}

static void mask_padding(IntBitset *set) {
    if(set!=NULL&&set->word_count>0) {
        set->words[set->word_count-1]&=final_mask(set);
    }
}

IntBitset *int_bitset_create(size_t bit_count) {
    IntBitset *set=calloc(1,sizeof *set);
    if(set==NULL)return NULL;

    set->bit_count=bit_count;
    set->word_count=words_for_bits(bit_count);

    if(set->word_count>0) {
        if(set->word_count>SIZE_MAX/sizeof *set->words) {
            free(set);
            return NULL;
        }

        set->words=calloc(set->word_count,sizeof *set->words);

        if(set->words==NULL) {
            free(set);
            return NULL;
        }
    }

    return set;
}

void int_bitset_free(IntBitset *set) {
    if(set==NULL)return;
    free(set->words);
    free(set);
}

size_t int_bitset_size(const IntBitset *set) {
    return set==NULL?0:set->bit_count;
}

static bool index_valid(const IntBitset *set,size_t index) {
    return set!=NULL&&index<set->bit_count;
}

bool int_bitset_test(
    const IntBitset *set,
    size_t index,
    bool *out_value
) {
    if(!index_valid(set,index)||out_value==NULL)return false;

    const size_t word=index/64U;
    const unsigned bit=(unsigned)(index%64U);

    *out_value=(set->words[word]&(UINT64_C(1)<<bit))!=0;
    return true;
}

bool int_bitset_set(IntBitset *set,size_t index) {
    if(!index_valid(set,index))return false;
    set->words[index/64U]|=UINT64_C(1)<<(index%64U);
    return true;
}

bool int_bitset_clear(IntBitset *set,size_t index) {
    if(!index_valid(set,index))return false;
    set->words[index/64U]&=~(UINT64_C(1)<<(index%64U));
    return true;
}

bool int_bitset_flip(IntBitset *set,size_t index) {
    if(!index_valid(set,index))return false;
    set->words[index/64U]^=UINT64_C(1)<<(index%64U);
    return true;
}

void int_bitset_clear_all(IntBitset *set) {
    if(set==NULL||set->word_count==0)return;
    memset(set->words,0,set->word_count*sizeof *set->words);
}

void int_bitset_set_all(IntBitset *set) {
    if(set==NULL||set->word_count==0)return;

    for(size_t i=0;i<set->word_count;++i) {
        set->words[i]=UINT64_MAX;
    }

    mask_padding(set);
}

void int_bitset_flip_all(IntBitset *set) {
    if(set==NULL)return;

    for(size_t i=0;i<set->word_count;++i) {
        set->words[i]=~set->words[i];
    }

    mask_padding(set);
}

size_t int_bitset_count(const IntBitset *set) {
    if(set==NULL)return 0;

    size_t count=0;

    for(size_t i=0;i<set->word_count;++i) {
        count+=(size_t)__builtin_popcountll(
            (unsigned long long)set->words[i]
        );
    }

    return count;
}

bool int_bitset_any(const IntBitset *set) {
    if(set==NULL)return false;

    for(size_t i=0;i<set->word_count;++i) {
        if(set->words[i]!=0)return true;
    }

    return false;
}

bool int_bitset_none(const IntBitset *set) {
    return !int_bitset_any(set);
}

bool int_bitset_all(const IntBitset *set) {
    if(set==NULL)return false;
    if(set->bit_count==0)return true;

    for(size_t i=0;i+1<set->word_count;++i) {
        if(set->words[i]!=UINT64_MAX)return false;
    }

    return set->words[set->word_count-1]==final_mask(set);
}

static bool same_shape(
    const IntBitset *out,
    const IntBitset *a,
    const IntBitset *b
) {
    return out!=NULL&&a!=NULL&&b!=NULL&&
           out->bit_count==a->bit_count&&
           a->bit_count==b->bit_count;
}

bool int_bitset_and(
    IntBitset *out,
    const IntBitset *a,
    const IntBitset *b
) {
    if(!same_shape(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]&b->words[i];
    }

    mask_padding(out);
    return true;
}

bool int_bitset_or(
    IntBitset *out,
    const IntBitset *a,
    const IntBitset *b
) {
    if(!same_shape(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]|b->words[i];
    }

    mask_padding(out);
    return true;
}

bool int_bitset_xor(
    IntBitset *out,
    const IntBitset *a,
    const IntBitset *b
) {
    if(!same_shape(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]^b->words[i];
    }

    mask_padding(out);
    return true;
}

bool int_bitset_not(
    IntBitset *out,
    const IntBitset *input
) {
    if(out==NULL||input==NULL||
       out->bit_count!=input->bit_count) {
        return false;
    }

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=~input->words[i];
    }

    mask_padding(out);
    return true;
}

bool int_bitset_find_next_set(
    const IntBitset *set,
    size_t start,
    size_t *out_index
) {
    if(set==NULL||out_index==NULL||start>=set->bit_count) {
        return false;
    }

    size_t word_index=start/64U;
    const unsigned offset=(unsigned)(start%64U);

    uint64_t word=set->words[word_index];

    if(offset>0U) {
        word&=UINT64_MAX<<offset;
    }

    for(;;) {
        if(word!=0) {
            const unsigned bit=(unsigned)__builtin_ctzll(
                (unsigned long long)word
            );
            const size_t index=word_index*64U+(size_t)bit;

            if(index<set->bit_count) {
                *out_index=index;
                return true;
            }

            return false;
        }

        ++word_index;

        if(word_index>=set->word_count)return false;
        word=set->words[word_index];
    }
}

bool int_bitset_validate(const IntBitset *set) {
    if(set==NULL)return false;

    if(set->word_count!=words_for_bits(set->bit_count))return false;

    if(set->word_count==0)return set->words==NULL;
    if(set->words==NULL)return false;

    const uint64_t padding_mask=~final_mask(set);

    return (set->words[set->word_count-1]&padding_mask)==0;
}
