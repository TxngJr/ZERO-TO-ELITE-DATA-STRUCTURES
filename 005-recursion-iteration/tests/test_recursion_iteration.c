#include "recursion_iteration.h"

#include <assert.h>
#include <stdio.h>

static void test_factorial(void) {
    const uint64_t expected[] = {
        1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, 720ULL,
        5040ULL, 40320ULL, 362880ULL, 3628800ULL
    };

    for (unsigned n = 0; n <= 10; ++n) {
        assert(factorial_recursive(n) == expected[n]);
        assert(factorial_iterative(n) == expected[n]);
    }
}

static void test_sum(void) {
    int data[128];
    for (size_t i = 0; i < 128; ++i) {
        data[i] = (int)i - 64;
    }

    for (size_t n = 0; n <= 128; ++n) {
        assert(sum_recursive(data, n) == sum_iterative(data, n));
    }
    assert(sum_recursive(NULL, 0) == 0);
}

static void test_gcd(void) {
    for (unsigned a = 0; a < 100; ++a) {
        for (unsigned b = 0; b < 100; ++b) {
            assert(gcd_recursive(a, b) == gcd_iterative(a, b));
        }
    }
}

static void test_binary_search(void) {
    const int data[] = {-10, -3, 0, 2, 5, 9, 12, 18, 25};
    const size_t n = sizeof data / sizeof data[0];

    for (size_t i = 0; i < n; ++i) {
        const ptrdiff_t r = binary_search_recursive(data, n, data[i]);
        const ptrdiff_t t = binary_search_iterative(data, n, data[i]);
        assert(r == (ptrdiff_t)i);
        assert(t == (ptrdiff_t)i);
    }

    const int missing[] = {-11, -2, 1, 7, 30};
    for (size_t i = 0; i < sizeof missing / sizeof missing[0]; ++i) {
        assert(binary_search_recursive(data, n, missing[i]) == -1);
        assert(binary_search_iterative(data, n, missing[i]) == -1);
    }

    assert(binary_search_recursive(NULL, 0, 1) == -1);
    assert(binary_search_iterative(NULL, 0, 1) == -1);
}

int main(void) {
    test_factorial();
    test_sum();
    test_gcd();
    test_binary_search();

    puts("recursion/iteration tests passed");
    return 0;
}
