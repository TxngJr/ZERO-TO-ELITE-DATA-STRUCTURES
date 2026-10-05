#include "int_bitmap.h"

#include <stdint.h>
#include <stdlib.h>

struct IntBitmap {
    size_t universe_size;
    size_t word_count;
    uint64_t *words;
    size_t cardinality;
};

static size_t words_for(size_t bits) {
    return bits/64U+(bits%64U!=0U?1U:0U);
}

static uint64_t last_mask(const IntBitmap *bitmap) {
    if(bitmap->universe_size==0)return 0;

    const unsigned used=(unsigned)(bitmap->universe_size%64U);

    if(used==0U)return UINT64_MAX;
    return (UINT64_C(1)<<used)-UINT64_C(1);
}

static size_t pop64(uint64_t word) {
    return (size_t)__builtin_popcountll(
        (unsigned long long)word
    );
}

static void mask_padding(IntBitmap *bitmap) {
    if(bitmap->word_count>0) {
        bitmap->words[bitmap->word_count-1]&=
            last_mask(bitmap);
    }
}

IntBitmap *int_bitmap_create(size_t universe_size) {
    IntBitmap *bitmap=calloc(1,sizeof *bitmap);
    if(bitmap==NULL)return NULL;

    bitmap->universe_size=universe_size;
    bitmap->word_count=words_for(universe_size);

    if(bitmap->word_count>0) {
        if(bitmap->word_count>SIZE_MAX/sizeof *bitmap->words) {
            free(bitmap);
            return NULL;
        }

        bitmap->words=calloc(
            bitmap->word_count,
            sizeof *bitmap->words
        );

        if(bitmap->words==NULL) {
            free(bitmap);
            return NULL;
        }
    }

    return bitmap;
}

void int_bitmap_free(IntBitmap *bitmap) {
    if(bitmap==NULL)return;
    free(bitmap->words);
    free(bitmap);
}

size_t int_bitmap_universe_size(const IntBitmap *bitmap) {
    return bitmap==NULL?0:bitmap->universe_size;
}

size_t int_bitmap_cardinality(const IntBitmap *bitmap) {
    return bitmap==NULL?0:bitmap->cardinality;
}

bool int_bitmap_contains(
    const IntBitmap *bitmap,
    size_t value,
    bool *out_present
) {
    if(bitmap==NULL||out_present==NULL||
       value>=bitmap->universe_size) {
        return false;
    }

    *out_present=(
        bitmap->words[value/64U]&
        (UINT64_C(1)<<(value%64U))
    )!=0;

    return true;
}

bool int_bitmap_add(IntBitmap *bitmap,size_t value) {
    if(bitmap==NULL||value>=bitmap->universe_size)return false;

    uint64_t *word=&bitmap->words[value/64U];
    const uint64_t mask=UINT64_C(1)<<(value%64U);

    if((*word&mask)==0) {
        *word|=mask;
        ++bitmap->cardinality;
    }

    return true;
}

bool int_bitmap_remove(IntBitmap *bitmap,size_t value) {
    if(bitmap==NULL||value>=bitmap->universe_size)return false;

    uint64_t *word=&bitmap->words[value/64U];
    const uint64_t mask=UINT64_C(1)<<(value%64U);

    if((*word&mask)!=0) {
        *word&=~mask;
        --bitmap->cardinality;
    }

    return true;
}

static uint64_t range_mask(unsigned lo,unsigned hi) {
    /* 0 <= lo < hi <= 64 */
    const uint64_t low=
        lo==0U?UINT64_MAX:(UINT64_MAX<<lo);

    const uint64_t high=
        hi==64U?UINT64_MAX:
        ((UINT64_C(1)<<hi)-UINT64_C(1));

    return low&high;
}

static bool mutate_range(
    IntBitmap *bitmap,
    size_t begin,
    size_t end,
    bool set_bits
) {
    if(bitmap==NULL||begin>end||
       end>bitmap->universe_size) {
        return false;
    }

    if(begin==end)return true;

    const size_t first_word=begin/64U;
    const size_t last_word=(end-1)/64U;

    for(size_t w=first_word;w<=last_word;++w) {
        const unsigned lo=w==first_word
            ?(unsigned)(begin%64U):0U;

        const unsigned hi=w==last_word
            ?(unsigned)(((end-1)%64U)+1U):64U;

        const uint64_t mask=range_mask(lo,hi);
        const uint64_t before=bitmap->words[w];
        const uint64_t after=set_bits
            ?(before|mask)
            :(before&~mask);

        const size_t old_count=pop64(before);
        const size_t new_count=pop64(after);

        if(new_count>=old_count) {
            bitmap->cardinality+=new_count-old_count;
        } else {
            bitmap->cardinality-=old_count-new_count;
        }

        bitmap->words[w]=after;
    }

    mask_padding(bitmap);
    return true;
}

bool int_bitmap_set_range(
    IntBitmap *bitmap,
    size_t begin,
    size_t end
) {
    return mutate_range(bitmap,begin,end,true);
}

bool int_bitmap_clear_range(
    IntBitmap *bitmap,
    size_t begin,
    size_t end
) {
    return mutate_range(bitmap,begin,end,false);
}

static bool compatible(
    const IntBitmap *out,
    const IntBitmap *a,
    const IntBitmap *b
) {
    return out&&a&&b&&
           out->universe_size==a->universe_size&&
           a->universe_size==b->universe_size;
}

static void recalc_cardinality(IntBitmap *bitmap) {
    size_t total=0;

    for(size_t i=0;i<bitmap->word_count;++i) {
        total+=pop64(bitmap->words[i]);
    }

    bitmap->cardinality=total;
}

bool int_bitmap_union(
    IntBitmap *out,
    const IntBitmap *a,
    const IntBitmap *b
) {
    if(!compatible(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]|b->words[i];
    }

    mask_padding(out);
    recalc_cardinality(out);
    return true;
}

bool int_bitmap_intersection(
    IntBitmap *out,
    const IntBitmap *a,
    const IntBitmap *b
) {
    if(!compatible(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]&b->words[i];
    }

    mask_padding(out);
    recalc_cardinality(out);
    return true;
}

bool int_bitmap_difference(
    IntBitmap *out,
    const IntBitmap *a,
    const IntBitmap *b
) {
    if(!compatible(out,a,b))return false;

    for(size_t i=0;i<out->word_count;++i) {
        out->words[i]=a->words[i]&~b->words[i];
    }

    mask_padding(out);
    recalc_cardinality(out);
    return true;
}

bool int_bitmap_next_member(
    const IntBitmap *bitmap,
    size_t start,
    size_t *out_value
) {
    if(bitmap==NULL||out_value==NULL||
       start>=bitmap->universe_size) {
        return false;
    }

    size_t w=start/64U;
    const unsigned offset=(unsigned)(start%64U);

    uint64_t word=bitmap->words[w];

    if(offset>0U)word&=UINT64_MAX<<offset;

    for(;;) {
        if(word!=0) {
            const unsigned bit=(unsigned)__builtin_ctzll(
                (unsigned long long)word
            );
            const size_t value=w*64U+(size_t)bit;

            if(value<bitmap->universe_size) {
                *out_value=value;
                return true;
            }

            return false;
        }

        ++w;
        if(w>=bitmap->word_count)return false;
        word=bitmap->words[w];
    }
}

bool int_bitmap_validate(const IntBitmap *bitmap) {
    if(bitmap==NULL)return false;

    if(bitmap->word_count!=words_for(bitmap->universe_size)) {
        return false;
    }

    if(bitmap->word_count==0) {
        return bitmap->words==NULL&&
               bitmap->cardinality==0;
    }

    if(bitmap->words==NULL||
       bitmap->cardinality>bitmap->universe_size) {
        return false;
    }

    if((bitmap->words[bitmap->word_count-1]&
        ~last_mask(bitmap))!=0) {
        return false;
    }

    size_t actual=0;

    for(size_t i=0;i<bitmap->word_count;++i) {
        actual+=pop64(bitmap->words[i]);
    }

    return actual==bitmap->cardinality;
}
