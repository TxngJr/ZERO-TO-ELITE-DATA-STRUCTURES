#include "int_binomial_heap.h"

#include <stdint.h>
#include <stdlib.h>

struct IntBinomialNode {
    int key;
    size_t degree;
    struct IntBinomialNode *parent;
    struct IntBinomialNode *child;
    struct IntBinomialNode *sibling;
};

struct IntBinomialHeap {
    IntBinomialNode *head;
    size_t size;
};

static IntBinomialNode *make_node(int key) {
    IntBinomialNode *node = calloc(1, sizeof *node);
    if (node != NULL) node->key = key;
    return node;
}

static void free_tree(IntBinomialNode *node) {
    if (node == NULL) return;

    IntBinomialNode *child = node->child;

    while (child != NULL) {
        IntBinomialNode *next = child->sibling;
        free_tree(child);
        child = next;
    }

    free(node);
}

IntBinomialHeap *int_binomial_heap_create(void) {
    return calloc(1, sizeof(IntBinomialHeap));
}

void int_binomial_heap_free(IntBinomialHeap *heap) {
    if (heap == NULL) return;

    IntBinomialNode *root = heap->head;

    while (root != NULL) {
        IntBinomialNode *next = root->sibling;
        free_tree(root);
        root = next;
    }

    free(heap);
}

size_t int_binomial_heap_size(const IntBinomialHeap *heap) {
    return heap == NULL ? 0 : heap->size;
}

static void link_tree(
    IntBinomialNode *child,
    IntBinomialNode *parent
) {
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
    ++parent->degree;
}

static IntBinomialNode *merge_root_lists(
    IntBinomialNode *a,
    IntBinomialNode *b
) {
    IntBinomialNode dummy = {0};
    IntBinomialNode *tail = &dummy;

    while (a != NULL && b != NULL) {
        if (a->degree <= b->degree) {
            IntBinomialNode *next = a->sibling;
            tail->sibling = a;
            tail = a;
            a = next;
        } else {
            IntBinomialNode *next = b->sibling;
            tail->sibling = b;
            tail = b;
            b = next;
        }
    }

    tail->sibling = a != NULL ? a : b;
    return dummy.sibling;
}

static IntBinomialNode *consolidate(IntBinomialNode *head) {
    if (head == NULL) return NULL;

    IntBinomialNode *prev = NULL;
    IntBinomialNode *current = head;
    IntBinomialNode *next = current->sibling;

    while (next != NULL) {
        const bool different_degree =
            current->degree != next->degree;

        const bool triple_same =
            next->sibling != NULL &&
            next->sibling->degree == current->degree;

        if (different_degree || triple_same) {
            prev = current;
            current = next;
        } else if (current->key <= next->key) {
            current->sibling = next->sibling;
            link_tree(next, current);
        } else {
            if (prev == NULL) {
                head = next;
            } else {
                prev->sibling = next;
            }

            link_tree(current, next);
            current = next;
        }

        next = current->sibling;
    }

    return head;
}

IntBinomialNode *int_binomial_heap_insert(
    IntBinomialHeap *heap,
    int key
) {
    if (heap == NULL || heap->size == SIZE_MAX) return NULL;

    IntBinomialNode *node = make_node(key);
    if (node == NULL) return NULL;

    heap->head = consolidate(merge_root_lists(heap->head, node));
    ++heap->size;

    return node;
}

bool int_binomial_heap_peek_min(
    const IntBinomialHeap *heap,
    int *out_key
) {
    if (heap == NULL || heap->head == NULL || out_key == NULL) {
        return false;
    }

    const IntBinomialNode *minimum = heap->head;

    for (const IntBinomialNode *root = heap->head->sibling;
         root != NULL;
         root = root->sibling) {
        if (root->key < minimum->key) {
            minimum = root;
        }
    }

    *out_key = minimum->key;
    return true;
}

static IntBinomialNode *reverse_children(
    IntBinomialNode *child
) {
    IntBinomialNode *reversed = NULL;

    while (child != NULL) {
        IntBinomialNode *next = child->sibling;

        child->parent = NULL;
        child->sibling = reversed;
        reversed = child;

        child = next;
    }

    return reversed;
}

static bool remove_root_pointer(
    IntBinomialHeap *heap,
    IntBinomialNode *target,
    int *out_key
) {
    IntBinomialNode *prev = NULL;
    IntBinomialNode *root = heap->head;

    while (root != NULL && root != target) {
        prev = root;
        root = root->sibling;
    }

    if (root == NULL) return false;

    if (prev == NULL) {
        heap->head = root->sibling;
    } else {
        prev->sibling = root->sibling;
    }

    IntBinomialNode *promoted = reverse_children(root->child);

    heap->head = consolidate(
        merge_root_lists(heap->head, promoted)
    );

    if (out_key != NULL) *out_key = root->key;

    free(root);
    --heap->size;
    return true;
}

bool int_binomial_heap_extract_min(
    IntBinomialHeap *heap,
    int *out_key
) {
    if (heap == NULL || heap->head == NULL) return false;

    IntBinomialNode *minimum = heap->head;

    for (IntBinomialNode *root = heap->head->sibling;
         root != NULL;
         root = root->sibling) {
        if (root->key < minimum->key) {
            minimum = root;
        }
    }

    return remove_root_pointer(heap, minimum, out_key);
}

bool int_binomial_heap_meld(
    IntBinomialHeap *destination,
    IntBinomialHeap *source
) {
    if (destination == NULL || source == NULL) return false;
    if (destination == source) return false;

    if (destination->size > SIZE_MAX - source->size) {
        return false;
    }

    destination->head = consolidate(
        merge_root_lists(destination->head, source->head)
    );
    destination->size += source->size;

    source->head = NULL;
    source->size = 0;

    return true;
}

bool int_binomial_node_decrease_key(
    IntBinomialNode *node,
    int new_key
) {
    if (node == NULL || new_key > node->key) return false;

    node->key = new_key;
    IntBinomialNode *cursor = node;

    while (cursor->parent != NULL &&
           cursor->key < cursor->parent->key) {
        const int tmp = cursor->key;
        cursor->key = cursor->parent->key;
        cursor->parent->key = tmp;

        cursor = cursor->parent;
    }

    return true;
}

static bool root_belongs_to_heap(
    const IntBinomialHeap *heap,
    const IntBinomialNode *root
) {
    for (const IntBinomialNode *cursor = heap->head;
         cursor != NULL;
         cursor = cursor->sibling) {
        if (cursor == root) return true;
    }

    return false;
}

bool int_binomial_heap_delete_handle(
    IntBinomialHeap *heap,
    IntBinomialNode *node
) {
    if (heap == NULL || node == NULL) return false;

    IntBinomialNode *root = node;

    while (root->parent != NULL) {
        root = root->parent;
    }

    if (!root_belongs_to_heap(heap, root)) return false;

    IntBinomialNode *cursor = node;

    while (cursor->parent != NULL) {
        const int tmp = cursor->key;
        cursor->key = cursor->parent->key;
        cursor->parent->key = tmp;

        cursor = cursor->parent;
    }

    return remove_root_pointer(heap, cursor, NULL);
}

int int_binomial_node_key(const IntBinomialNode *node) {
    return node == NULL ? 0 : node->key;
}

static bool validate_tree(
    const IntBinomialNode *node,
    const IntBinomialNode *parent,
    size_t expected_degree,
    size_t limit,
    size_t *count
) {
    if (node == NULL) return false;
    if (node->parent != parent) return false;
    if (node->degree != expected_degree) return false;

    if (*count >= limit) return false;
    ++*count;

    size_t remaining = expected_degree;
    const IntBinomialNode *child = node->child;

    while (child != NULL) {
        if (remaining == 0) return false;
        --remaining;

        if (child->degree != remaining) return false;
        if (node->key > child->key) return false;

        if (!validate_tree(
                child,
                node,
                remaining,
                limit,
                count
            )) {
            return false;
        }

        child = child->sibling;
    }

    return remaining == 0;
}

bool int_binomial_heap_validate(
    const IntBinomialHeap *heap
) {
    if (heap == NULL) return false;

    if (heap->size == 0) {
        return heap->head == NULL;
    }

    if (heap->head == NULL) return false;

    size_t count = 0;
    bool first = true;
    size_t previous_degree = 0;

    for (const IntBinomialNode *root = heap->head;
         root != NULL;
         root = root->sibling) {
        if (root->parent != NULL) return false;

        if (!first && root->degree <= previous_degree) {
            return false;
        }

        if (!validate_tree(
                root,
                NULL,
                root->degree,
                heap->size,
                &count
            )) {
            return false;
        }

        previous_degree = root->degree;
        first = false;
    }

    return count == heap->size;
}
