#include "int_dsu.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t next_rng(uint32_t *state) {
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

static void naive_union(
    size_t *label,
    size_t n,
    size_t a,
    size_t b
) {
    const size_t from = label[b];
    const size_t to = label[a];

    if (from == to) return;

    for (size_t i = 0; i < n; ++i) {
        if (label[i] == from) label[i] = to;
    }
}

static size_t naive_components(
    const size_t *label,
    size_t n
) {
    size_t count = 0;

    for (size_t i = 0; i < n; ++i) {
        bool first = true;

        for (size_t j = 0; j < i; ++j) {
            if (label[j] == label[i]) {
                first = false;
                break;
            }
        }

        if (first) ++count;
    }

    return count;
}

static size_t naive_size(
    const size_t *label,
    size_t n,
    size_t element
) {
    size_t count = 0;

    for (size_t i = 0; i < n; ++i) {
        if (label[i] == label[element]) ++count;
    }

    return count;
}

static void test_basic(void) {
    IntDSU *dsu = int_dsu_create(6);
    assert(dsu != NULL);
    assert(int_dsu_component_count(dsu) == 6);

    bool merged = false;

    assert(int_dsu_union(dsu, 0, 1, &merged) && merged);
    assert(int_dsu_union(dsu, 2, 3, &merged) && merged);
    assert(int_dsu_union(dsu, 1, 3, &merged) && merged);
    assert(int_dsu_union(dsu, 0, 2, &merged) && !merged);

    bool connected = false;
    assert(int_dsu_connected(dsu, 0, 3, &connected) && connected);
    assert(int_dsu_connected(dsu, 0, 5, &connected) && !connected);

    size_t size = 0;
    assert(int_dsu_component_size(dsu, 2, &size));
    assert(size == 4);

    assert(int_dsu_component_count(dsu) == 3);
    assert(int_dsu_validate(dsu));

    int_dsu_free(dsu);
}

static void test_randomized(void) {
    enum { N = 256, STEPS = 50000 };

    IntDSU *dsu = int_dsu_create(N);
    assert(dsu != NULL);

    size_t *label = malloc(N * sizeof *label);
    assert(label != NULL);

    for (size_t i = 0; i < N; ++i) label[i] = i;

    uint32_t rng = 0xD5A12345u;

    for (int step = 0; step < STEPS; ++step) {
        const size_t a = next_rng(&rng) % N;
        const size_t b = next_rng(&rng) % N;
        const unsigned op = next_rng(&rng) % 4U;

        if (op <= 1U) {
            const bool already = label[a] == label[b];

            bool merged = false;
            assert(int_dsu_union(dsu, a, b, &merged));
            assert(merged == !already);

            naive_union(label, N, a, b);
        } else if (op == 2U) {
            bool connected = false;
            assert(int_dsu_connected(dsu, a, b, &connected));
            assert(connected == (label[a] == label[b]));
        } else {
            size_t size = 0;
            assert(int_dsu_component_size(dsu, a, &size));
            assert(size == naive_size(label, N, a));
        }

        if ((step % 257) == 0) {
            assert(int_dsu_component_count(dsu) ==
                   naive_components(label, N));
            assert(int_dsu_validate(dsu));
        }
    }

    assert(int_dsu_validate(dsu));

    free(label);
    int_dsu_free(dsu);
}

int main(void) {
    test_basic();
    test_randomized();

    puts("DSU tests passed");
    return 0;
}
