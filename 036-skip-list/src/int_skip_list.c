#include "int_skip_list.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct SkipNode {
    int key;
    size_t level;
    struct SkipNode *next[];
} SkipNode;

struct IntSkipList {
    SkipNode *head;
    size_t max_level;
    size_t current_level;
    size_t size;
    uint64_t rng_state;
};

static SkipNode *node_create(
    int key,
    size_t level
) {
    if (level == 0) return NULL;

    if (level >
        (SIZE_MAX - sizeof(SkipNode)) /
            sizeof(SkipNode *)) {
        return NULL;
    }

    SkipNode *node = calloc(
        1,
        sizeof(SkipNode) +
            level * sizeof(SkipNode *)
    );

    if (node == NULL) return NULL;

    node->key = key;
    node->level = level;
    return node;
}

IntSkipList *int_skip_list_create(
    size_t max_level
) {
    if (max_level == 0) return NULL;

    IntSkipList *list = calloc(1, sizeof *list);
    if (list == NULL) return NULL;

    list->head = node_create(0, max_level);

    if (list->head == NULL) {
        free(list);
        return NULL;
    }

    list->max_level = max_level;
    list->current_level = 1;
    list->rng_state =
        UINT64_C(0x9e3779b97f4a7c15);

    return list;
}

void int_skip_list_free(IntSkipList *list) {
    if (list == NULL) return;

    SkipNode *node = list->head->next[0];

    while (node != NULL) {
        SkipNode *next = node->next[0];
        free(node);
        node = next;
    }

    free(list->head);
    free(list);
}

size_t int_skip_list_size(
    const IntSkipList *list
) {
    return list == NULL ? 0 : list->size;
}

size_t int_skip_list_current_level(
    const IntSkipList *list
) {
    return list == NULL ? 0 : list->current_level;
}

size_t int_skip_list_max_level(
    const IntSkipList *list
) {
    return list == NULL ? 0 : list->max_level;
}

static uint64_t rng_next(IntSkipList *list) {
    uint64_t x = list->rng_state;

    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;

    list->rng_state = x;
    return x * UINT64_C(2685821657736338717);
}

static size_t random_level(
    IntSkipList *list
) {
    size_t level = 1;

    while (level < list->max_level) {
        if ((rng_next(list) & 1U) == 0U) {
            break;
        }
        ++level;
    }

    return level;
}

bool int_skip_list_contains(
    const IntSkipList *list,
    int key
) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    const SkipNode *node = list->head;

    for (size_t level = list->current_level;
         level > 0;
         --level) {
        const size_t index = level - 1U;

        while (node->next[index] != NULL &&
               node->next[index]->key < key) {
            node = node->next[index];
        }
    }

    node = node->next[0];

    return node != NULL && node->key == key;
}

bool int_skip_list_insert(
    IntSkipList *list,
    int key
) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    if (list->max_level >
        SIZE_MAX / sizeof(SkipNode *)) {
        return false;
    }

    SkipNode **update = malloc(
        list->max_level * sizeof *update
    );
    if (update == NULL) return false;

    SkipNode *node = list->head;

    for (size_t level = list->current_level;
         level > 0;
         --level) {
        const size_t index = level - 1U;

        while (node->next[index] != NULL &&
               node->next[index]->key < key) {
            node = node->next[index];
        }

        update[index] = node;
    }

    node = node->next[0];

    if (node != NULL && node->key == key) {
        free(update);
        return false;
    }

    const size_t node_level = random_level(list);

    if (node_level > list->current_level) {
        for (size_t i = list->current_level;
             i < node_level;
             ++i) {
            update[i] = list->head;
        }

        list->current_level = node_level;
    }

    SkipNode *created = node_create(
        key,
        node_level
    );

    if (created == NULL) {
        while (list->current_level > 1 &&
               list->head->next[
                   list->current_level - 1U
               ] == NULL) {
            --list->current_level;
        }

        free(update);
        return false;
    }

    for (size_t i = 0; i < node_level; ++i) {
        created->next[i] = update[i]->next[i];
        update[i]->next[i] = created;
    }

    ++list->size;
    free(update);
    return true;
}

bool int_skip_list_remove(
    IntSkipList *list,
    int key
) {
    if (list == NULL || list->head == NULL) {
        return false;
    }

    if (list->max_level >
        SIZE_MAX / sizeof(SkipNode *)) {
        return false;
    }

    SkipNode **update = malloc(
        list->max_level * sizeof *update
    );
    if (update == NULL) return false;

    SkipNode *node = list->head;

    for (size_t level = list->current_level;
         level > 0;
         --level) {
        const size_t index = level - 1U;

        while (node->next[index] != NULL &&
               node->next[index]->key < key) {
            node = node->next[index];
        }

        update[index] = node;
    }

    SkipNode *target = node->next[0];

    if (target == NULL || target->key != key) {
        free(update);
        return false;
    }

    for (size_t i = 0;
         i < target->level;
         ++i) {
        if (update[i]->next[i] == target) {
            update[i]->next[i] =
                target->next[i];
        }
    }

    free(target);
    --list->size;

    while (list->current_level > 1 &&
           list->head->next[
               list->current_level - 1U
           ] == NULL) {
        --list->current_level;
    }

    free(update);
    return true;
}

bool int_skip_list_range(
    const IntSkipList *list,
    int low,
    int high,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (list == NULL || out_written == NULL) {
        return false;
    }

    *out_written = 0;

    if (low > high || list->size == 0) {
        return true;
    }

    const SkipNode *node = list->head;

    for (size_t level = list->current_level;
         level > 0;
         --level) {
        const size_t index = level - 1U;

        while (node->next[index] != NULL &&
               node->next[index]->key < low) {
            node = node->next[index];
        }
    }

    node = node->next[0];

    size_t needed = 0;
    const SkipNode *cursor = node;

    while (cursor != NULL &&
           cursor->key <= high) {
        ++needed;
        cursor = cursor->next[0];
    }

    if (needed > capacity ||
        (needed > 0 && output == NULL)) {
        return false;
    }

    while (node != NULL &&
           node->key <= high) {
        output[(*out_written)++] = node->key;
        node = node->next[0];
    }

    return true;
}

static bool level_zero_contains_pointer(
    const IntSkipList *list,
    const SkipNode *target
) {
    const SkipNode *node = list->head->next[0];

    while (node != NULL) {
        if (node == target) return true;
        node = node->next[0];
    }

    return false;
}

bool int_skip_list_validate(
    const IntSkipList *list
) {
    if (list == NULL ||
        list->head == NULL ||
        list->max_level == 0 ||
        list->current_level == 0 ||
        list->current_level > list->max_level ||
        list->head->level != list->max_level) {
        return false;
    }

    size_t level_zero_count = 0;
    const SkipNode *node = list->head->next[0];
    bool first = true;
    int previous = 0;

    while (node != NULL) {
        if (node->level == 0 ||
            node->level > list->max_level) {
            return false;
        }

        if (!first && node->key <= previous) {
            return false;
        }

        previous = node->key;
        first = false;

        ++level_zero_count;
        if (level_zero_count > list->size) {
            return false;
        }

        node = node->next[0];
    }

    if (level_zero_count != list->size) {
        return false;
    }

    for (size_t level = 1;
         level < list->current_level;
         ++level) {
        const SkipNode *cursor =
            list->head->next[level];

        first = true;

        while (cursor != NULL) {
            if (cursor->level <= level) {
                return false;
            }

            if (!first && cursor->key <= previous) {
                return false;
            }

            if (!level_zero_contains_pointer(
                    list,
                    cursor
                )) {
                return false;
            }

            previous = cursor->key;
            first = false;

            cursor = cursor->next[level];
        }
    }

    for (size_t level = list->current_level;
         level < list->max_level;
         ++level) {
        if (list->head->next[level] != NULL) {
            return false;
        }
    }

    if (list->size == 0) {
        return list->current_level == 1 &&
               list->head->next[0] == NULL;
    }

    return list->head->next[
        list->current_level - 1U
    ] != NULL;
}
