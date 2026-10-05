#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char tag;
    int value;
    double weight;
} Record;

static void show_stack_object(void) {
    int local = 123;
    printf("local value=%d address=%p\n", local, (void *)&local);
}

int main(void) {
    int local = 7;
    int *heap_value = malloc(sizeof *heap_value);
    if (heap_value == NULL) {
        fputs("allocation failed\n", stderr);
        return 1;
    }

    *heap_value = 99;

    Record record = {.tag = 'A', .value = 42, .weight = 3.5};

    printf("main local address=%p\n", (void *)&local);
    printf("heap pointer value=%p pointee=%d\n", (void *)heap_value, *heap_value);
    printf("record address=%p sizeof(Record)=%zu\n", (void *)&record, sizeof record);

    show_stack_object();

    free(heap_value);
    heap_value = NULL;

    return 0;
}
