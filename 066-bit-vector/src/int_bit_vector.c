#include "int_bit_vector.h"

#include <stdint.h>
#include <stdlib.h>

struct IntBitVector {
    size_t bit_count;
    size_t word_count;
    uint64_t *words;
    size_t *prefix_ones;
};

static size_t words_for(size_t bits) {
    return bits/64U+(bits%64U!=0U?1U:0U);
}

static size_t pop64(uint64_t word) {
    return (size_t)__builtin_popcountll(
        (unsigned long long)word
    );
}

static uint64_t final_mask(const IntBitVector *vector) {
    if(vector->bit_count==0)return 0;

    const unsigned used=(unsigned)(vector->bit_count%64U);

    if(used==0U)return UINT64_MAX;

    return (UINT64_C(1)<<used)-UINT64_C(1);
}

IntBitVector *int_bit_vector_create(
    const uint8_t *bits,
    size_t bit_count
) {
    if(bit_count>0&&bits==NULL)return NULL;

    IntBitVector *vector=calloc(1,sizeof *vector);
    if(vector==NULL)return NULL;

    vector->bit_count=bit_count;
    vector->word_count=words_for(bit_count);

    if(vector->word_count>0) {
        if(vector->word_count>SIZE_MAX/sizeof *vector->words) {
            int_bit_vector_free(vector);
            return NULL;
        }

        vector->words=calloc(
            vector->word_count,
            sizeof *vector->words
        );

        if(vector->words==NULL) {
            int_bit_vector_free(vector);
            return NULL;
        }
    }

    if(vector->word_count==SIZE_MAX||
       vector->word_count+1>SIZE_MAX/sizeof *vector->prefix_ones) {
        int_bit_vector_free(vector);
        return NULL;
    }

    vector->prefix_ones=calloc(
        vector->word_count+1,
        sizeof *vector->prefix_ones
    );

    if(vector->prefix_ones==NULL) {
        int_bit_vector_free(vector);
        return NULL;
    }

    for(size_t i=0;i<bit_count;++i) {
        if(bits[i]>1U) {
            int_bit_vector_free(vector);
            return NULL;
        }

        if(bits[i]!=0U) {
            vector->words[i/64U]|=
                UINT64_C(1)<<(i%64U);
        }
    }

    for(size_t w=0;w<vector->word_count;++w) {
        const size_t count=pop64(vector->words[w]);

        if(SIZE_MAX-vector->prefix_ones[w]<count) {
            int_bit_vector_free(vector);
            return NULL;
        }

        vector->prefix_ones[w+1]=
            vector->prefix_ones[w]+count;
    }

    return vector;
}

void int_bit_vector_free(IntBitVector *vector) {
    if(vector==NULL)return;
    free(vector->words);
    free(vector->prefix_ones);
    free(vector);
}

size_t int_bit_vector_size(const IntBitVector *vector) {
    return vector==NULL?0:vector->bit_count;
}

size_t int_bit_vector_ones(const IntBitVector *vector) {
    if(vector==NULL||vector->prefix_ones==NULL)return 0;
    return vector->prefix_ones[vector->word_count];
}

size_t int_bit_vector_zeros(const IntBitVector *vector) {
    if(vector==NULL)return 0;
    return vector->bit_count-int_bit_vector_ones(vector);
}

bool int_bit_vector_access(
    const IntBitVector *vector,
    size_t index,
    bool *out_bit
) {
    if(vector==NULL||out_bit==NULL||
       index>=vector->bit_count) {
        return false;
    }

    *out_bit=(
        vector->words[index/64U]&
        (UINT64_C(1)<<(index%64U))
    )!=0;

    return true;
}

bool int_bit_vector_rank1(
    const IntBitVector *vector,
    size_t end,
    size_t *out_rank
) {
    if(vector==NULL||out_rank==NULL||
       end>vector->bit_count) {
        return false;
    }

    if(end==vector->bit_count&&end%64U==0U) {
        *out_rank=vector->prefix_ones[vector->word_count];
        return true;
    }

    const size_t word=end/64U;
    const unsigned offset=(unsigned)(end%64U);

    if(offset==0U) {
        *out_rank=vector->prefix_ones[word];
        return true;
    }

    const uint64_t mask=
        (UINT64_C(1)<<offset)-UINT64_C(1);

    *out_rank=vector->prefix_ones[word]+
        pop64(vector->words[word]&mask);

    return true;
}

bool int_bit_vector_rank0(
    const IntBitVector *vector,
    size_t end,
    size_t *out_rank
) {
    if(out_rank==NULL)return false;

    size_t ones=0;

    if(!int_bit_vector_rank1(vector,end,&ones))return false;

    *out_rank=end-ones;
    return true;
}

static size_t bits_before_word(
    const IntBitVector *vector,
    size_t word
) {
    if(word>=vector->word_count)return vector->bit_count;

    const size_t bits=word*64U;
    return bits<vector->bit_count?bits:vector->bit_count;
}

static size_t zero_prefix(
    const IntBitVector *vector,
    size_t word
) {
    return bits_before_word(vector,word)-
           vector->prefix_ones[word];
}

static bool select_in_word(
    uint64_t word,
    size_t local_k,
    size_t word_index,
    size_t bit_count,
    size_t *out_index
) {
    while(local_k>0) {
        word&=word-UINT64_C(1);
        --local_k;
    }

    if(word==0)return false;

    const unsigned bit=(unsigned)__builtin_ctzll(
        (unsigned long long)word
    );

    const size_t index=word_index*64U+(size_t)bit;

    if(index>=bit_count)return false;

    *out_index=index;
    return true;
}

bool int_bit_vector_select1(
    const IntBitVector *vector,
    size_t k,
    size_t *out_index
) {
    if(vector==NULL||out_index==NULL||
       k>=int_bit_vector_ones(vector)) {
        return false;
    }

    size_t low=0;
    size_t high=vector->word_count;

    while(low<high) {
        const size_t mid=low+(high-low)/2;

        if(vector->prefix_ones[mid+1]<=k) {
            low=mid+1;
        } else {
            high=mid;
        }
    }

    const size_t word=low;
    const size_t local=k-vector->prefix_ones[word];

    return select_in_word(
        vector->words[word],
        local,word,vector->bit_count,out_index
    );
}

bool int_bit_vector_select0(
    const IntBitVector *vector,
    size_t k,
    size_t *out_index
) {
    if(vector==NULL||out_index==NULL||
       k>=int_bit_vector_zeros(vector)) {
        return false;
    }

    size_t low=0;
    size_t high=vector->word_count;

    while(low<high) {
        const size_t mid=low+(high-low)/2;
        const size_t zeros_after=
            zero_prefix(vector,mid+1);

        if(zeros_after<=k) {
            low=mid+1;
        } else {
            high=mid;
        }
    }

    const size_t word=low;
    const size_t local=k-zero_prefix(vector,word);

    uint64_t zeros=~vector->words[word];

    if(word+1==vector->word_count) {
        zeros&=final_mask(vector);
    }

    return select_in_word(
        zeros,local,word,vector->bit_count,out_index
    );
}

bool int_bit_vector_validate(const IntBitVector *vector) {
    if(vector==NULL||vector->prefix_ones==NULL)return false;

    if(vector->word_count!=words_for(vector->bit_count))return false;

    if(vector->word_count==0) {
        return vector->words==NULL&&
               vector->prefix_ones[0]==0;
    }

    if(vector->words==NULL||
       vector->prefix_ones[0]!=0) {
        return false;
    }

    if((vector->words[vector->word_count-1]&
        ~final_mask(vector))!=0) {
        return false;
    }

    for(size_t w=0;w<vector->word_count;++w) {
        const size_t count=pop64(vector->words[w]);

        if(vector->prefix_ones[w+1]<
           vector->prefix_ones[w]||
           vector->prefix_ones[w+1]-
           vector->prefix_ones[w]!=count) {
            return false;
        }
    }

    return vector->prefix_ones[vector->word_count]
        <=vector->bit_count;
}
