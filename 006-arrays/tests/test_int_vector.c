#include "int_vector.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void assert_matches(const IntVector *vector, const int *ref, size_t ref_size) {
    assert(int_vector_size(vector) == ref_size);
    assert(int_vector_capacity(vector) >= ref_size);

    for (size_t i = 0; i < ref_size; ++i) {
        int value = 0;
        assert(int_vector_get(vector, i, &value));
        assert(value == ref[i]);
    }

    int unused = 0;
    assert(!int_vector_get(vector, ref_size, &unused));

    const int *data = int_vector_data(vector);
    if (ref_size > 0) {
        assert(data != NULL);
        for (size_t i = 0; i < ref_size; ++i) {
            assert(data[i] == ref[i]);
        }
    }
}

static void test_basic_operations(void) {
    IntVector *v = int_vector_create();
    assert(v != NULL);
    assert(int_vector_is_empty(v));
    assert(int_vector_size(v) == 0);

    for (int i = 0; i < 1000; ++i) {
        assert(int_vector_push(v, i * 3));
        assert(int_vector_capacity(v) >= int_vector_size(v));
    }

    assert(int_vector_size(v) == 1000);

    int value = 0;
    assert(int_vector_get(v, 500, &value));
    assert(value == 1500);

    assert(int_vector_set(v, 500, -7));
    assert(int_vector_get(v, 500, &value));
    assert(value == -7);

    for (int i = 999; i >= 501; --i) {
        assert(int_vector_pop(v, &value));
        assert(value == i * 3);
    }

    assert(int_vector_pop(v, &value));
    assert(value == -7);

    int_vector_free(v);
}

static void test_insert_erase_clear_shrink(void) {
    IntVector *v = int_vector_create();
    assert(v != NULL);

    assert(int_vector_push(v, 10));
    assert(int_vector_push(v, 20));
    assert(int_vector_push(v, 30));

    assert(int_vector_insert(v, 0, 5));
    assert(int_vector_insert(v, 2, 15));
    assert(int_vector_insert(v, int_vector_size(v), 40));

    const int expected[] = {5, 10, 15, 20, 30, 40};
    assert_matches(v, expected, 6);

    int removed = 0;
    assert(int_vector_erase(v, 2, &removed));
    assert(removed == 15);

    const int expected2[] = {5, 10, 20, 30, 40};
    assert_matches(v, expected2, 5);

    assert(int_vector_shrink_to_fit(v));
    assert(int_vector_capacity(v) == int_vector_size(v));

    int_vector_clear(v);
    assert(int_vector_is_empty(v));
    assert(int_vector_capacity(v) == 5);

    assert(int_vector_shrink_to_fit(v));
    assert(int_vector_capacity(v) == 0);
    assert(int_vector_data(v) == NULL);

    int_vector_free(v);
}

static uint32_t next_random(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void test_randomized_differential(void) {
    enum { MAX_REF = 512, STEPS = 10000 };

    IntVector *v = int_vector_create();
    assert(v != NULL);

    int ref[MAX_REF];
    size_t ref_size = 0;
    uint32_t rng = 0xC0FFEEu;

    for (int step = 0; step < STEPS; ++step) {
        uint32_t r = next_random(&rng);
        unsigned op = r % 5U;

        if ((op == 0U || ref_size == 0) && ref_size < MAX_REF) {
            int value = (int)next_random(&rng);
            assert(int_vector_push(v, value));
            ref[ref_size++] = value;
        } else if (op == 1U && ref_size < MAX_REF) {
            size_t index = next_random(&rng) % (ref_size + 1);
            int value = (int)next_random(&rng);
            assert(int_vector_insert(v, index, value));
            memmove(&ref[index + 1], &ref[index], (ref_size - index) * sizeof ref[0]);
            ref[index] = value;
            ++ref_size;
        } else if (op == 2U && ref_size > 0) {
            size_t index = next_random(&rng) % ref_size;
            int expected = ref[index];
            int removed = 0;
            assert(int_vector_erase(v, index, &removed));
            assert(removed == expected);
            memmove(&ref[index], &ref[index + 1], (ref_size - index - 1) * sizeof ref[0]);
            --ref_size;
        } else if (op == 3U && ref_size > 0) {
            size_t index = next_random(&rng) % ref_size;
            int value = (int)next_random(&rng);
            assert(int_vector_set(v, index, value));
            ref[index] = value;
        } else if (ref_size > 0) {
            int removed = 0;
            assert(int_vector_pop(v, &removed));
            assert(removed == ref[ref_size - 1]);
            --ref_size;
        }

        assert_matches(v, ref, ref_size);
    }

    int_vector_free(v);
}

static void test_invalid_inputs(void) {
    int out = 0;

    assert(int_vector_size(NULL) == 0);
    assert(int_vector_capacity(NULL) == 0);
    assert(int_vector_is_empty(NULL));
    assert(!int_vector_push(NULL, 1));
    assert(!int_vector_pop(NULL, &out));
    assert(!int_vector_get(NULL, 0, &out));
    assert(!int_vector_set(NULL, 0, 1));
    assert(!int_vector_insert(NULL, 0, 1));
    assert(!int_vector_erase(NULL, 0, &out));
    assert(!int_vector_reserve(NULL, 10));
    assert(!int_vector_shrink_to_fit(NULL));
    assert(int_vector_data(NULL) == NULL);

    IntVector *v = int_vector_create();
    assert(v != NULL);
    assert(!int_vector_get(v, 0, &out));
    assert(!int_vector_get(v, 0, NULL));
    assert(!int_vector_set(v, 0, 1));
    assert(!int_vector_insert(v, 1, 1));
    assert(!int_vector_erase(v, 0, &out));
    assert(!int_vector_pop(v, &out));
    int_vector_free(v);

    int_vector_free(NULL);
}

int main(void) {
    test_basic_operations();
    test_insert_erase_clear_shrink();
    test_randomized_differential();
    test_invalid_inputs();

    puts("IntVector tests passed");
    return 0;
}
