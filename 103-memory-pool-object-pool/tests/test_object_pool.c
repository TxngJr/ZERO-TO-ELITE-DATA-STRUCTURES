#include "object_pool.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

enum { CAPACITY = 1024, STEPS = 100000 };

static uint64_t state = UINT64_C(0x9e3779b97f4a7c15);
static uint64_t rng(void) {
    state ^= state << 7;
    state ^= state >> 9;
    state ^= state << 8;
    return state;
}

int main(void) {
    ObjectPool *pool = op_create(24U, CAPACITY);
    assert(pool);
    assert(op_validate(pool));

    void *slots[CAPACITY] = {0};
    size_t live = 0U;

    for (size_t step = 0; step < STEPS; ++step) {
        bool do_alloc =
            live == 0U ||
            (live < CAPACITY && (rng() % 100U) < 58U);

        if (do_alloc) {
            void *ptr = op_alloc(pool);
            assert(ptr);
            memset(ptr, (int)(step & 255U), 24U);

            size_t pos = 0U;
            while (pos < CAPACITY && slots[pos]) ++pos;
            assert(pos < CAPACITY);
            slots[pos] = ptr;
            ++live;
        } else {
            size_t kth = (size_t)(rng() % live);
            size_t pos = 0U;
            while (pos < CAPACITY) {
                if (slots[pos]) {
                    if (kth == 0U) break;
                    --kth;
                }
                ++pos;
            }
            assert(pos < CAPACITY);

            void *ptr = slots[pos];
            assert(op_owns(pool, ptr));
            assert(op_release(pool, ptr));
            assert(!op_release(pool, ptr));
            slots[pos] = NULL;
            --live;
        }

        assert(op_in_use_count(pool) == live);
        if (step % 997U == 0U)
            assert(op_validate(pool));
    }

    for (size_t i = 0; i < CAPACITY; ++i)
        if (slots[i])
            assert(op_release(pool, slots[i]));

    int foreign = 0;
    assert(!op_release(pool, &foreign));
    assert(op_in_use_count(pool) == 0U);
    assert(op_validate(pool));

    void *a = op_alloc(pool);
    assert(a);
    assert(op_release(pool, a));
    void *b = op_alloc(pool);
    assert(b == a);
    assert(op_release(pool, b));

    printf("Object pool tests passed; stride=%zu capacity=%zu\n",
           op_stride(pool), op_capacity(pool));
    op_free(pool);
    return 0;
}
