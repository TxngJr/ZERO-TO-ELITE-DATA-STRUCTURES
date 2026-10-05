#include "int_deque.h"
#include "monotonic_queue.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    IntDeque *d = int_deque_create();
    assert(d != NULL);

    assert(int_deque_push_back(d, 10));
    assert(int_deque_push_front(d, 5));
    assert(int_deque_push_back(d, 20));

    int front = 0;
    int back = 0;
    assert(int_deque_front(d, &front));
    assert(int_deque_back(d, &back));
    printf("front=%d back=%d size=%zu\n", front, back, int_deque_size(d));

    const int input[] = {1, 3, -1, -3, 5, 3, 6, 7};
    int output[6];
    assert(sliding_window_maximum(input, 8, 3, output));

    puts("window max:");
    for (size_t i = 0; i < 6; ++i) printf("%d\n", output[i]);

    int_deque_free(d);
    return 0;
}
