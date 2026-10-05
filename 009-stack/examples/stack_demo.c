#include "array_stack.h"
#include "linked_stack.h"
#include "monotonic_stack.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    ArrayStack *array = array_stack_create();
    LinkedStack *linked = linked_stack_create();
    assert(array != NULL && linked != NULL);

    for (int i = 1; i <= 3; ++i) {
        assert(array_stack_push(array, i * 10));
        assert(linked_stack_push(linked, i * 10));
    }

    int a = 0;
    int b = 0;
    assert(array_stack_pop(array, &a));
    assert(linked_stack_pop(linked, &b));
    printf("popped array=%d linked=%d\n", a, b);

    const int input[] = {2, 1, 5, 3, 4};
    int output[5];
    assert(next_greater_values(input, 5, -1, output));

    puts("next greater:");
    for (size_t i = 0; i < 5; ++i) {
        printf("%d -> %d\n", input[i], output[i]);
    }

    array_stack_free(array);
    linked_stack_free(linked);
    return 0;
}
