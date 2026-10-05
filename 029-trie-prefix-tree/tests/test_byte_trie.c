#include "byte_trie.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    unsigned char keys[64][16];
    size_t lengths[64];
    size_t count;
} Collector;

static bool collect_key(
    const unsigned char *bytes,
    size_t length,
    void *context
) {
    Collector *collector = context;
    assert(collector->count < 64);
    assert(length <= 16);

    memcpy(collector->keys[collector->count], bytes, length);
    collector->lengths[collector->count] = length;
    ++collector->count;
    return true;
}

static void test_prefix_and_delete(void) {
    ByteTrie *trie = byte_trie_create();
    assert(trie != NULL);

    const char *words[] = {
        "app","apple","application","apt","cat","car"
    };

    for (size_t i = 0; i < 6; ++i) {
        assert(byte_trie_insert(
            trie,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    assert(byte_trie_contains(trie,(const unsigned char *)"app",3));
    assert(!byte_trie_contains(trie,(const unsigned char *)"ap",2));
    assert(byte_trie_has_prefix(trie,(const unsigned char *)"ap",2));
    assert(byte_trie_count_prefix(trie,(const unsigned char *)"ap",2)==4);

    assert(byte_trie_remove(trie,(const unsigned char *)"app",3));
    assert(!byte_trie_contains(trie,(const unsigned char *)"app",3));
    assert(byte_trie_contains(trie,(const unsigned char *)"apple",5));
    assert(byte_trie_count_prefix(trie,(const unsigned char *)"ap",2)==3);
    assert(byte_trie_validate(trie));

    byte_trie_free(trie);
}

static void test_empty_and_binary(void) {
    ByteTrie *trie = byte_trie_create();
    assert(trie != NULL);

    assert(byte_trie_insert(trie,NULL,0));
    assert(byte_trie_contains(trie,NULL,0));
    assert(byte_trie_has_prefix(trie,NULL,0));

    const unsigned char binary[] = {0x41,0x00,0x42};
    assert(byte_trie_insert(trie,binary,3));
    assert(byte_trie_contains(trie,binary,3));
    assert(byte_trie_validate(trie));

    assert(byte_trie_remove(trie,NULL,0));
    assert(!byte_trie_contains(trie,NULL,0));

    byte_trie_free(trie);
}

static void test_lexicographic(void) {
    ByteTrie *trie = byte_trie_create();
    assert(trie != NULL);

    const char *words[] = {"dog","app","apt","apple","car","cat"};

    for (size_t i = 0; i < 6; ++i) {
        assert(byte_trie_insert(
            trie,
            (const unsigned char *)words[i],
            strlen(words[i])
        ));
    }

    Collector c = {0};

    assert(byte_trie_visit_prefix(trie,NULL,0,collect_key,&c));
    assert(c.count == 6);

    const char *expected[] = {"app","apple","apt","car","cat","dog"};

    for (size_t i = 0; i < 6; ++i) {
        assert(c.lengths[i] == strlen(expected[i]));
        assert(memcmp(c.keys[i],expected[i],c.lengths[i])==0);
    }

    byte_trie_free(trie);
}

static void test_generated_keys(void) {
    ByteTrie *trie = byte_trie_create();
    assert(trie != NULL);

    enum { N=1000 };

    char key[32];

    for (int i = 0; i < N; ++i) {
        const int length = snprintf(key,sizeof key,"key/%04d",i);
        assert(length > 0);
        assert(byte_trie_insert(
            trie,
            (const unsigned char *)key,
            (size_t)length
        ));
    }

    assert(byte_trie_size(trie)==N);
    assert(byte_trie_count_prefix(
        trie,
        (const unsigned char *)"key/0",
        5
    )==1000);

    for (int i = 0; i < N; i += 2) {
        const int length = snprintf(key,sizeof key,"key/%04d",i);
        assert(byte_trie_remove(
            trie,
            (const unsigned char *)key,
            (size_t)length
        ));
    }

    assert(byte_trie_size(trie)==500);
    assert(byte_trie_validate(trie));

    byte_trie_free(trie);
}

int main(void) {
    test_prefix_and_delete();
    test_empty_and_binary();
    test_lexicographic();
    test_generated_keys();

    puts("Byte Trie tests passed");
    return 0;
}
