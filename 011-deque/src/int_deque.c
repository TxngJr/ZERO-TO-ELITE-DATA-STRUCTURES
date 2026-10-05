#include "int_deque.h"

#include <stdint.h>
#include <stdlib.h>

struct IntDeque {
    int *data;
    size_t capacity;
    size_t head;
    size_t size;
};

static size_t advance(size_t index, size_t offset, size_t capacity) {
    if (capacity == 0) return 0;
    offset %= capacity;
    const size_t room = capacity - index;
    return offset < room ? index + offset : offset - room;
}

static size_t previous(size_t index, size_t capacity) {
    return index == 0 ? capacity - 1 : index - 1;
}

static bool grow(IntDeque *deque, size_t need) {
    if (need <= deque->capacity) return true;

    size_t new_capacity = deque->capacity == 0 ? 8 : deque->capacity;
    while (new_capacity < need) {
        if (new_capacity > SIZE_MAX / 2) {
            new_capacity = need;
            break;
        }
        new_capacity *= 2;
    }

    if (new_capacity > SIZE_MAX / sizeof *deque->data) return false;

    int *new_data = malloc(new_capacity * sizeof *new_data);
    if (new_data == NULL) return false;

    for (size_t i = 0; i < deque->size; ++i) {
        new_data[i] = deque->data[advance(deque->head, i, deque->capacity)];
    }

    free(deque->data);
    deque->data = new_data;
    deque->capacity = new_capacity;
    deque->head = 0;
    return true;
}

IntDeque *int_deque_create(void) {
    return calloc(1, sizeof(IntDeque));
}

void int_deque_free(IntDeque *deque) {
    if (deque == NULL) return;
    free(deque->data);
    free(deque);
}

size_t int_deque_size(const IntDeque *deque) {
    return deque == NULL ? 0 : deque->size;
}

bool int_deque_is_empty(const IntDeque *deque) {
    return int_deque_size(deque) == 0;
}

bool int_deque_push_front(IntDeque *deque, int value) {
    if (deque == NULL || deque->size == SIZE_MAX) return false;
    if (!grow(deque, deque->size + 1)) return false;

    deque->head = previous(deque->head, deque->capacity);
    deque->data[deque->head] = value;
    ++deque->size;
    return true;
}

bool int_deque_push_back(IntDeque *deque, int value) {
    if (deque == NULL || deque->size == SIZE_MAX) return false;
    if (!grow(deque, deque->size + 1)) return false;

    const size_t index = advance(deque->head, deque->size, deque->capacity);
    deque->data[index] = value;
    ++deque->size;
    return true;
}

bool int_deque_pop_front(IntDeque *deque, int *out) {
    if (deque == NULL || deque->size == 0) return false;

    const int value = deque->data[deque->head];
    deque->head = advance(deque->head, 1, deque->capacity);
    --deque->size;

    if (deque->size == 0) deque->head = 0;
    if (out != NULL) *out = value;
    return true;
}

bool int_deque_pop_back(IntDeque *deque, int *out) {
    if (deque == NULL || deque->size == 0) return false;

    const size_t index = advance(deque->head, deque->size - 1, deque->capacity);
    const int value = deque->data[index];
    --deque->size;

    if (deque->size == 0) deque->head = 0;
    if (out != NULL) *out = value;
    return true;
}

bool int_deque_front(const IntDeque *deque, int *out) {
    if (deque == NULL || deque->size == 0 || out == NULL) return false;
    *out = deque->data[deque->head];
    return true;
}

bool int_deque_back(const IntDeque *deque, int *out) {
    if (deque == NULL || deque->size == 0 || out == NULL) return false;
    const size_t index = advance(deque->head, deque->size - 1, deque->capacity);
    *out = deque->data[index];
    return true;
}

bool int_deque_get(const IntDeque *deque, size_t index, int *out) {
    if (deque == NULL || index >= deque->size || out == NULL) return false;
    *out = deque->data[advance(deque->head, index, deque->capacity)];
    return true;
}

bool int_deque_validate(const IntDeque *deque) {
    if (deque == NULL) return false;
    if (deque->size > deque->capacity) return false;

    if (deque->capacity == 0) {
        return deque->data == NULL && deque->size == 0 && deque->head == 0;
    }

    if (deque->data == NULL || deque->head >= deque->capacity) return false;
    return true;
}
