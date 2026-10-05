#include "int_bitmap.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint32_t next_rng(uint32_t *state) {
    *state=*state*1664525u+1013904223u;
    return *state;
}

static size_t model_count(const uint8_t *model,size_t n) {
    size_t c=0;
    for(size_t i=0;i<n;++i)c+=model[i]!=0;
    return c;
}

static void check(
    const IntBitmap *bitmap,
    const uint8_t *model,
    size_t n
) {
    assert(int_bitmap_validate(bitmap));
    assert(int_bitmap_cardinality(bitmap)==model_count(model,n));

    for(size_t i=0;i<n;++i) {
        bool present=false;
        assert(int_bitmap_contains(bitmap,i,&present));
        assert(present==(model[i]!=0));
    }
}

static void test_ranges(void) {
    enum { N=130 };
    IntBitmap *bitmap=int_bitmap_create(N);
    uint8_t model[N];
    memset(model,0,sizeof model);

    assert(bitmap!=NULL);

    assert(int_bitmap_set_range(bitmap,10,80));
    for(size_t i=10;i<80;++i)model[i]=1;

    assert(int_bitmap_clear_range(bitmap,20,30));
    for(size_t i=20;i<30;++i)model[i]=0;

    assert(int_bitmap_set_range(bitmap,63,66));
    for(size_t i=63;i<66;++i)model[i]=1;

    check(bitmap,model,N);
    int_bitmap_free(bitmap);
}

static void test_set_algebra(void) {
    enum { N=130 };
    IntBitmap *a=int_bitmap_create(N);
    IntBitmap *b=int_bitmap_create(N);
    IntBitmap *out=int_bitmap_create(N);

    assert(a&&b&&out);

    for(size_t i=0;i<N;i+=2)assert(int_bitmap_add(a,i));
    for(size_t i=0;i<N;i+=3)assert(int_bitmap_add(b,i));

    assert(int_bitmap_intersection(out,a,b));
    assert(int_bitmap_cardinality(out)==22);

    assert(int_bitmap_union(out,a,b));
    assert(int_bitmap_validate(out));

    assert(int_bitmap_difference(out,a,b));

    for(size_t i=0;i<N;++i) {
        bool v=false;
        assert(int_bitmap_contains(out,i,&v));
        assert(v==(i%2==0&&i%3!=0));
    }

    int_bitmap_free(out);
    int_bitmap_free(b);
    int_bitmap_free(a);
}

static void test_randomized(void) {
    enum { N=257, OPS=40000 };
    IntBitmap *bitmap=int_bitmap_create(N);
    uint8_t model[N];
    memset(model,0,sizeof model);

    assert(bitmap!=NULL);

    uint32_t rng=0x65A12345u;

    for(int op=0;op<OPS;++op) {
        const unsigned kind=next_rng(&rng)%5U;

        if(kind<2) {
            const size_t i=next_rng(&rng)%N;

            if(kind==0) {
                assert(int_bitmap_add(bitmap,i));
                model[i]=1;
            } else {
                assert(int_bitmap_remove(bitmap,i));
                model[i]=0;
            }
        } else if(kind<4) {
            size_t a=next_rng(&rng)%(N+1U);
            size_t b=next_rng(&rng)%(N+1U);

            if(a>b) {
                const size_t t=a;a=b;b=t;
            }

            if(kind==2) {
                assert(int_bitmap_set_range(bitmap,a,b));
                for(size_t i=a;i<b;++i)model[i]=1;
            } else {
                assert(int_bitmap_clear_range(bitmap,a,b));
                for(size_t i=a;i<b;++i)model[i]=0;
            }
        } else {
            const size_t start=next_rng(&rng)%N;
            size_t expected=N;

            for(size_t i=start;i<N;++i) {
                if(model[i]) {
                    expected=i;
                    break;
                }
            }

            size_t actual=0;
            const bool found=int_bitmap_next_member(
                bitmap,start,&actual
            );

            assert(found==(expected!=N));
            if(found)assert(actual==expected);
        }

        if((op%701)==0)check(bitmap,model,N);
    }

    check(bitmap,model,N);
    int_bitmap_free(bitmap);
}

int main(void) {
    IntBitmap *empty=int_bitmap_create(0);
    assert(empty!=NULL);
    assert(int_bitmap_validate(empty));
    int_bitmap_free(empty);

    test_ranges();
    test_set_algebra();
    test_randomized();

    puts("Bitmap tests passed");
    return 0;
}
