#include "byte_tst.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char values[64][32];
    size_t count;
} Collector;

static bool collect(
    const unsigned char *bytes,
    size_t length,
    void *context
) {
    Collector *c = context;
    assert(c->count < 64);
    assert(length < 32);

    if (length > 0) memcpy(c->values[c->count], bytes, length);
    c->values[c->count][length] = '\0';
    ++c->count;
    return true;
}

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_basic(void) {
    ByteTST *tree = byte_tst_create();
    assert(tree != NULL);

    const char *words[]={"car","cat","dog","door","doom"};

    for(size_t i=0;i<5;++i) {
        assert(byte_tst_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    assert(byte_tst_contains(tree,(const unsigned char *)"car",3));
    assert(!byte_tst_contains(tree,(const unsigned char *)"ca",2));
    assert(byte_tst_has_prefix(tree,(const unsigned char *)"ca",2));
    assert(byte_tst_count_prefix(tree,(const unsigned char *)"do",2)==3);

    assert(byte_tst_remove(tree,(const unsigned char *)"cat",3));
    assert(!byte_tst_contains(tree,(const unsigned char *)"cat",3));
    assert(byte_tst_contains(tree,(const unsigned char *)"car",3));
    assert(byte_tst_validate(tree));

    byte_tst_free(tree);
}

static void test_empty_binary(void) {
    ByteTST *tree=byte_tst_create();
    assert(tree!=NULL);

    assert(byte_tst_insert(tree,NULL,0));
    assert(byte_tst_contains(tree,NULL,0));

    const unsigned char key[]={0x41,0x00,0x42};
    assert(byte_tst_insert(tree,key,3));
    assert(byte_tst_contains(tree,key,3));
    assert(byte_tst_validate(tree));

    byte_tst_free(tree);
}

static void test_order(void) {
    ByteTST *tree=byte_tst_create();
    assert(tree!=NULL);

    const char *words[]={"dog","app","apple","apt","car","cat"};

    for(size_t i=0;i<6;++i) {
        assert(byte_tst_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    Collector c={0};
    assert(byte_tst_visit_prefix(tree,NULL,0,collect,&c));

    const char *expected[]={"app","apple","apt","car","cat","dog"};
    assert(c.count==6);

    for(size_t i=0;i<6;++i) {
        assert(strcmp(c.values[i],expected[i])==0);
    }

    byte_tst_free(tree);
}

static void test_randomized(void) {
    enum { UNIVERSE=500, STEPS=20000 };

    ByteTST *tree=byte_tst_create();
    assert(tree!=NULL);

    bool present[UNIVERSE];
    memset(present,0,sizeof present);

    uint32_t rng=0x31A55123u;
    char key[32];
    size_t expected_size=0;

    for(int step=0;step<STEPS;++step) {
        const int id=(int)(next_rng(&rng)%UNIVERSE);
        const int len=snprintf(key,sizeof key,"item/%03d",id);
        assert(len>0);

        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const bool inserted=byte_tst_insert(
                tree,
                (const unsigned char *)key,
                (size_t)len
            );
            assert(inserted==!present[id]);
            if(inserted) {
                present[id]=true;
                ++expected_size;
            }
        } else if(op==1U) {
            const bool removed=byte_tst_remove(
                tree,
                (const unsigned char *)key,
                (size_t)len
            );
            assert(removed==present[id]);
            if(removed) {
                present[id]=false;
                --expected_size;
            }
        } else {
            assert(byte_tst_contains(
                tree,
                (const unsigned char *)key,
                (size_t)len
            )==present[id]);
        }

        assert(byte_tst_size(tree)==expected_size);
        assert(byte_tst_validate(tree));
    }

    byte_tst_free(tree);
}

int main(void) {
    test_basic();
    test_empty_binary();
    test_order();
    test_randomized();

    puts("Ternary Search Tree tests passed");
    return 0;
}
