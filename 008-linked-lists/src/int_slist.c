#include "int_slist.h"

#include <stdlib.h>

typedef struct SNode {
    int value;
    struct SNode *next;
} SNode;

struct IntSList {
    SNode *head;
    SNode *tail;
    size_t size;
};

static SNode *make_node(int value) {
    SNode *node = malloc(sizeof *node);

    if (node != NULL) {
        node->value = value;
        node->next = NULL;
    }

    return node;
}

IntSList *int_slist_create(void) {
    return calloc(1, sizeof(IntSList));
}

void int_slist_free(IntSList *list) {
    if (list == NULL) {
        return;
    }

    SNode *node = list->head;

    while (node != NULL) {
        SNode *next = node->next;
        free(node);
        node = next;
    }

    free(list);
}

size_t int_slist_size(const IntSList *list) {
    return list == NULL ? 0 : list->size;
}

bool int_slist_push_front(IntSList *list, int value) {
    if (list == NULL) {
        return false;
    }

    SNode *node = make_node(value);
    if (node == NULL) {
        return false;
    }

    node->next = list->head;
    list->head = node;

    if (list->size == 0) {
        list->tail = node;
    }

    ++list->size;
    return true;
}

bool int_slist_push_back(IntSList *list, int value) {
    if (list == NULL) {
        return false;
    }

    SNode *node = make_node(value);
    if (node == NULL) {
        return false;
    }

    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }

    list->tail = node;
    ++list->size;
    return true;
}

bool int_slist_pop_front(IntSList *list, int *out) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    SNode *victim = list->head;
    list->head = victim->next;

    if (out != NULL) {
        *out = victim->value;
    }

    free(victim);
    --list->size;

    if (list->size == 0) {
        list->tail = NULL;
    }

    return true;
}

bool int_slist_pop_back(IntSList *list, int *out) {
    if (list == NULL || list->tail == NULL) {
        return false;
    }

    if (list->size == 1) {
        return int_slist_pop_front(list, out);
    }

    SNode *previous = list->head;
    while (previous->next != list->tail) {
        previous = previous->next;
    }

    SNode *victim = list->tail;

    if (out != NULL) {
        *out = victim->value;
    }

    previous->next = NULL;
    list->tail = previous;

    free(victim);
    --list->size;
    return true;
}

static SNode *node_at(const IntSList *list, size_t index) {
    if (list == NULL || index >= list->size) {
        return NULL;
    }

    SNode *node = list->head;

    for (size_t i = 0; i < index; ++i) {
        node = node->next;
    }

    return node;
}

bool int_slist_get(const IntSList *list, size_t index, int *out) {
    if (out == NULL) {
        return false;
    }

    SNode *node = node_at(list, index);

    if (node == NULL) {
        return false;
    }

    *out = node->value;
    return true;
}

bool int_slist_insert(IntSList *list, size_t index, int value) {
    if (list == NULL || index > list->size) {
        return false;
    }

    if (index == 0) {
        return int_slist_push_front(list, value);
    }

    if (index == list->size) {
        return int_slist_push_back(list, value);
    }

    SNode *previous = node_at(list, index - 1);
    SNode *node = make_node(value);

    if (node == NULL) {
        return false;
    }

    node->next = previous->next;
    previous->next = node;

    ++list->size;
    return true;
}

bool int_slist_erase(IntSList *list, size_t index, int *out) {
    if (list == NULL || index >= list->size) {
        return false;
    }

    if (index == 0) {
        return int_slist_pop_front(list, out);
    }

    SNode *previous = node_at(list, index - 1);
    SNode *victim = previous->next;

    previous->next = victim->next;

    if (victim == list->tail) {
        list->tail = previous;
    }

    if (out != NULL) {
        *out = victim->value;
    }

    free(victim);
    --list->size;

    return true;
}

long long int_slist_sum(const IntSList *list) {
    if (list == NULL) {
        return 0;
    }

    long long sum = 0;

    for (const SNode *node = list->head; node != NULL; node = node->next) {
        sum += node->value;
    }

    return sum;
}

bool int_slist_validate(const IntSList *list) {
    if (list == NULL) {
        return false;
    }

    if (list->size == 0) {
        return list->head == NULL && list->tail == NULL;
    }

    if (list->head == NULL || list->tail == NULL || list->tail->next != NULL) {
        return false;
    }

    size_t count = 0;
    const SNode *node = list->head;
    const SNode *last = NULL;

    while (node != NULL && count <= list->size) {
        last = node;
        node = node->next;
        ++count;
    }

    return node == NULL && count == list->size && last == list->tail;
}
