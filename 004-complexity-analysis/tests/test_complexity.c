#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static size_t triangular(size_t n) {
    size_t count = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            ++count;
        }
    }
    return count;
}

static size_t halving(size_t n) {
    size_t count = 0;
    while (n > 1) {
        n /= 2;
        ++count;
    }
    return count;
}

int main(void) {
    for (size_t n = 0; n < 100; ++n) {
        assert(triangular(n) == n * (n + 1) / 2);
    }

    assert(halving(1) == 0);
    assert(halving(2) == 1);
    assert(halving(4) == 2);
    assert(halving(8) == 3);
    assert(halving(64) == 6);

    puts("complexity-count tests passed");
    return 0;
}
