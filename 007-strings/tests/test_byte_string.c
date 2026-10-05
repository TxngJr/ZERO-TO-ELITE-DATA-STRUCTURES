#include "byte_string.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void assert_equals(const ByteString *s, const char *bytes, size_t n) {
    assert(byte_string_length(s) == n);
    assert(byte_string_capacity(s) >= n);

    const char *data = byte_string_c_str(s);
    assert(data != NULL);
    assert(memcmp(data, bytes, n) == 0);
    assert(data[n] == '\0');
}

static void test_basic(void) {
    ByteString *s = byte_string_create();
    assert(s != NULL);
    assert_equals(s, "", 0);

    assert(byte_string_append_bytes(s, "hello", 5));
    assert(byte_string_append_char(s, '!'));
    assert_equals(s, "hello!", 6);

    assert(byte_string_insert_bytes(s, 5, " world", 6));
    assert_equals(s, "hello world!", 12);

    assert(byte_string_find(s, "world", 5) == 6);
    assert(byte_string_find(s, "missing", 7) == -1);
    assert(byte_string_find(s, "", 0) == 0);

    assert(byte_string_erase(s, 5, 6));
    assert_equals(s, "hello!", 6);

    byte_string_clear(s);
    assert_equals(s, "", 0);

    byte_string_free(s);
}

static void test_aliasing(void) {
    ByteString *s = byte_string_from_bytes("abc", 3);
    assert(s != NULL);

    const char *data = byte_string_c_str(s);
    assert(byte_string_append_bytes(s, data, 3));
    assert_equals(s, "abcabc", 6);

    data = byte_string_c_str(s);
    assert(byte_string_insert_bytes(s, 1, data + 2, 3));
    assert_equals(s, "acabbcabc", 9);

    byte_string_free(s);
}

static uint32_t rng_next(uint32_t *state) {
    *state = *state * 1103515245u + 12345u;
    return *state;
}

static void test_randomized_append_erase(void) {
    enum { MAX = 2048, STEPS = 5000 };

    ByteString *s = byte_string_create();
    assert(s != NULL);

    char reference[MAX + 1];
    size_t length = 0;
    reference[0] = '\0';

    uint32_t rng = 0x12345678u;

    for (int step = 0; step < STEPS; ++step) {
        const uint32_t r = rng_next(&rng);

        if ((r & 1U) == 0U && length < MAX) {
            const char ch = (char)('a' + (rng_next(&rng) % 26U));

            assert(byte_string_append_char(s, ch));
            reference[length++] = ch;
            reference[length] = '\0';
        } else if (length > 0) {
            const size_t index = rng_next(&rng) % length;
            size_t count = 1 + (rng_next(&rng) % 8U);

            if (count > length - index) {
                count = length - index;
            }

            assert(byte_string_erase(s, index, count));

            memmove(
                reference + index,
                reference + index + count,
                length - index - count + 1
            );

            length -= count;
        }

        assert_equals(s, reference, length);
    }

    byte_string_free(s);
}

static void test_embedded_zero(void) {
    const char bytes[] = {'a', '\0', 'b'};

    ByteString *s = byte_string_from_bytes(bytes, 3);
    assert(s != NULL);

    assert(byte_string_length(s) == 3);
    assert(memcmp(byte_string_c_str(s), bytes, 3) == 0);
    assert(byte_string_c_str(s)[3] == '\0');

    byte_string_free(s);
}

static void test_invalid_inputs(void) {
    assert(byte_string_from_bytes(NULL, 1) == NULL);

    ByteString *s = byte_string_create();
    assert(s != NULL);

    assert(!byte_string_append_bytes(NULL, "x", 1));
    assert(!byte_string_append_bytes(s, NULL, 1));
    assert(!byte_string_insert_bytes(s, 1, "x", 1));
    assert(!byte_string_erase(NULL, 0, 1));

    byte_string_free(s);
    byte_string_free(NULL);
}

int main(void) {
    test_basic();
    test_aliasing();
    test_randomized_append_erase();
    test_embedded_zero();
    test_invalid_inputs();

    puts("ByteString tests passed");
    return 0;
}
