#include "heap_sort.h"

static void swap_int(int *a, int *b) {
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void max_sift_down(int *values, size_t size, size_t index) {
    if (size < 2) return;

    while (index <= (size - 2) / 2) {
        const size_t left = index * 2 + 1;
        const size_t right = left + 1;
        size_t largest = left;

        if (right < size && values[right] > values[left]) {
            largest = right;
        }

        if (values[index] >= values[largest]) break;

        swap_int(&values[index], &values[largest]);
        index = largest;
    }
}

void int_heap_sort_ascending(int *values, size_t count) {
    if (values == NULL || count < 2) return;

    for (size_t i = count / 2; i > 0; --i) {
        max_sift_down(values, count, i - 1);
    }

    for (size_t end = count; end > 1; --end) {
        swap_int(&values[0], &values[end - 1]);
        max_sift_down(values, end - 1, 0);
    }
}
