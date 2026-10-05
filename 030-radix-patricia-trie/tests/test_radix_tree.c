#include "byte_radix_tree.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char values[64][32];
    size_t lengths[64];
    size_t count;
} Collector;

static bool collect(
    const unsigned char *bytes,
    size_t length,
    void *context
) {
    Collector *collector = context;

    assert(collector->count < 64);
    assert(length < 32);

    memcpy(collector->values[collector->count], bytes, length);
    collector->values[collector->count][length] = '\0';
    collector->lengths[collector->count] = length;
    ++collector->count;

    return true;
}

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_split_prefix_delete(void) {
    ByteRadixTree *tree = byte_radix_tree_create();
    assert(tree != NULL);

    const char *words[] = {
        "carpet","carbon","car","cat","application","apple"
    };

    for (size_t i = 0; i < 6; ++i) {
        assert(byte_radix_tree_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
        assert(byte_radix_tree_validate(tree));
    }

    assert(byte_radix_tree_contains(tree,(const unsigned char *)"car",3));
    assert(!byte_radix_tree_contains(tree,(const unsigned char *)"ca",2));
    assert(byte_radix_tree_has_prefix(tree,(const unsigned char *)"ca",2));
    assert(byte_radix_tree_has_prefix(tree,(const unsigned char *)"app",3));

    assert(byte_radix_tree_count_prefix(
        tree,
        (const unsigned char *)"car",
        3
    )==3);

    assert(byte_radix_tree_remove(
        tree,
        (const unsigned char *)"carbon",
        6
    ));

    assert(!byte_radix_tree_contains(
        tree,
        (const unsigned char *)"carbon",
        6
    ));
    assert(byte_radix_tree_contains(
        tree,
        (const unsigned char *)"carpet",
        6
    ));
    assert(byte_radix_tree_validate(tree));

    byte_radix_tree_free(tree);
}

static void test_binary_and_empty(void) {
    ByteRadixTree *tree = byte_radix_tree_create();
    assert(tree != NULL);

    assert(byte_radix_tree_insert(tree,NULL,0));
    assert(byte_radix_tree_contains(tree,NULL,0));

    const unsigned char a[] = {0x41,0x00,0x42,0x43};
    const unsigned char prefix[] = {0x41,0x00};

    assert(byte_radix_tree_insert(tree,a,4));
    assert(byte_radix_tree_contains(tree,a,4));
    assert(byte_radix_tree_has_prefix(tree,prefix,2));
    assert(byte_radix_tree_validate(tree));

    byte_radix_tree_free(tree);
}

static void test_lexicographic(void) {
    ByteRadixTree *tree = byte_radix_tree_create();
    assert(tree != NULL);

    const char *words[] = {"dog","app","apple","apt","car","cat"};

    for (size_t i=0;i<6;++i) {
        assert(byte_radix_tree_insert(
            tree,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    Collector collector={0};

    assert(byte_radix_tree_visit_prefix(
        tree,
        NULL,
        0,
        collect,
        &collector
    ));

    const char *expected[]={"app","apple","apt","car","cat","dog"};

    assert(collector.count==6);

    for(size_t i=0;i<6;++i) {
        assert(strcmp(collector.values[i],expected[i])==0);
    }

    byte_radix_tree_free(tree);
}

static void test_randomized_universe(void) {
    enum { UNIVERSE=500, STEPS=20000 };

    ByteRadixTree *tree=byte_radix_tree_create();
    assert(tree!=NULL);

    bool present[UNIVERSE];
    memset(present,0,sizeof present);

    uint32_t rng=0x30AD1A55u;
    char key[32];
    size_t expected_size=0;

    for(int step=0;step<STEPS;++step) {
        const int id=(int)(next_rng(&rng)%UNIVERSE);
        const int len=snprintf(key,sizeof key,"prefix/shared/%03d",id);
        assert(len>0);

        const unsigned op=next_rng(&rng)%3U;

        if(op==0U) {
            const bool inserted=byte_radix_tree_insert(
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
            const bool removed=byte_radix_tree_remove(
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
            assert(byte_radix_tree_contains(
                tree,
                (const unsigned char *)key,
                (size_t)len
            )==present[id]);
        }

        assert(byte_radix_tree_size(tree)==expected_size);
        assert(byte_radix_tree_validate(tree));
    }

    byte_radix_tree_free(tree);
}

int main(void) {
    test_split_prefix_delete();
    test_binary_and_empty();
    test_lexicographic();
    test_randomized_universe();

    puts("Radix Tree tests passed");
    return 0;
}
