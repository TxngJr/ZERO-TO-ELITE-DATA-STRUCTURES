#include "recursion_iteration.h"

uint64_t factorial_recursive(unsigned n) {
    if (n <= 1U) {
        return 1U;
    }
    return (uint64_t)n * factorial_recursive(n - 1U);
}

uint64_t factorial_iterative(unsigned n) {
    uint64_t result = 1U;
    for (unsigned i = 2U; i <= n; ++i) {
        result *= (uint64_t)i;
    }
    return result;
}

long long sum_recursive(const int *data, size_t n) {
    if (n == 0) {
        return 0;
    }
    return sum_recursive(data, n - 1) + data[n - 1];
}

long long sum_iterative(const int *data, size_t n) {
    long long sum = 0;
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
    }
    return sum;
}

unsigned gcd_recursive(unsigned a, unsigned b) {
    if (b == 0U) {
        return a;
    }
    return gcd_recursive(b, a % b);
}

unsigned gcd_iterative(unsigned a, unsigned b) {
    while (b != 0U) {
        const unsigned remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

static ptrdiff_t binary_search_recursive_range(
    const int *data,
    size_t lo,
    size_t hi,
    int target
) {
    if (lo >= hi) {
        return -1;
    }

    const size_t mid = lo + (hi - lo) / 2;
    if (data[mid] == target) {
        return (ptrdiff_t)mid;
    }
    if (target < data[mid]) {
        return binary_search_recursive_range(data, lo, mid, target);
    }
    return binary_search_recursive_range(data, mid + 1, hi, target);
}

ptrdiff_t binary_search_recursive(const int *data, size_t n, int target) {
    if (data == NULL && n != 0) {
        return -1;
    }
    return binary_search_recursive_range(data, 0, n, target);
}

ptrdiff_t binary_search_iterative(const int *data, size_t n, int target) {
    if (data == NULL && n != 0) {
        return -1;
    }

    size_t lo = 0;
    size_t hi = n;

    while (lo < hi) {
        const size_t mid = lo + (hi - lo) / 2;
        if (data[mid] == target) {
            return (ptrdiff_t)mid;
        }
        if (target < data[mid]) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return -1;
}
