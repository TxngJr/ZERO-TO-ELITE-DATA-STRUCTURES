#include "int_slist.h"
#include "int_vector.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

static double elapsed(struct timespec a, struct timespec b) {
    return (double)(b.tv_sec - a.tv_sec) +
           (double)(b.tv_nsec - a.tv_nsec) / 1000000000.0;
}

int main(void) {
    const size_t n = 200000;

    IntSList *list = int_slist_create();
    IntVector *vector = int_vector_create();

    if (list == NULL || vector == NULL) {
        int_slist_free(list);
        int_vector_free(vector);
        return 1;
    }

    for (size_t i = 0; i < n; ++i) {
        if (!int_slist_push_back(list, (int)i) ||
            !int_vector_push(vector, (int)i)) {
            int_slist_free(list);
            int_vector_free(vector);
            return 1;
        }
    }

    struct timespec a, b;
    volatile int64_t vector_sum = 0;
    volatile int64_t list_sum = 0;

    const int *data = int_vector_data(vector);

    timespec_get(&a, TIME_UTC);
    for (size_t i = 0; i < n; ++i) {
        vector_sum += data[i];
    }
    timespec_get(&b, TIME_UTC);
    const double vector_time = elapsed(a, b);

    timespec_get(&a, TIME_UTC);
    list_sum = int_slist_sum(list);
    timespec_get(&b, TIME_UTC);
    const double list_time = elapsed(a, b);

    printf("direct vector traversal: %.6f s\n", vector_time);
    printf("direct singly-list traversal: %.6f s\n", list_time);
    printf("sums: %lld %lld\n",
           (long long)vector_sum,
           (long long)list_sum);

    puts("Both traversals are Theta(n); timing differences reflect representation/hardware effects.");
    puts("Index-based int_slist_get(i) repeated in a loop would be Theta(n^2), which is a different API-cost experiment.");

    int_slist_free(list);
    int_vector_free(vector);

    return vector_sum == list_sum ? 0 : 1;
}
