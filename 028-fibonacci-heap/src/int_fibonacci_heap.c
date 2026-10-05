#include "int_fibonacci_heap.h"

#include <stdint.h>
#include <stdlib.h>

struct IntFibonacciNode {
    int key;
    size_t degree;
    bool mark;
    struct IntFibonacciNode *parent;
    struct IntFibonacciNode *child;
    struct IntFibonacciNode *left;
    struct IntFibonacciNode *right;
};

struct IntFibonacciHeap {
    IntFibonacciNode *min;
    size_t size;
};

static IntFibonacciNode *make_node(int key) {
    IntFibonacciNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->key = key;
    node->left = node;
    node->right = node;
    return node;
}

static void list_insert_after(
    IntFibonacciNode *position,
    IntFibonacciNode *node
) {
    node->right = position->right;
    node->left = position;
    position->right->left = node;
    position->right = node;
}

static void list_remove(IntFibonacciNode *node) {
    node->left->right = node->right;
    node->right->left = node->left;
    node->left = node;
    node->right = node;
}

static void concatenate_lists(
    IntFibonacciNode *a,
    IntFibonacciNode *b
) {
    IntFibonacciNode *a_right = a->right;
    IntFibonacciNode *b_left = b->left;

    a->right = b;
    b->left = a;

    a_right->left = b_left;
    b_left->right = a_right;
}

static void add_root(
    IntFibonacciHeap *heap,
    IntFibonacciNode *node
) {
    node->parent = NULL;
    node->mark = false;

    if (heap->min == NULL) {
        node->left = node;
        node->right = node;
        heap->min = node;
        return;
    }

    list_insert_after(heap->min, node);

    if (node->key < heap->min->key) {
        heap->min = node;
    }
}

static void free_node_tree(IntFibonacciNode *node);

static void free_ring(IntFibonacciNode *start) {
    if (start == NULL) return;

    IntFibonacciNode *current = start;

    for (;;) {
        IntFibonacciNode *next = current->right;
        const bool last = next == start;

        free_node_tree(current);

        if (last) break;
        current = next;
    }
}

static void free_node_tree(IntFibonacciNode *node) {
    if (node->child != NULL) {
        free_ring(node->child);
    }
    free(node);
}

IntFibonacciHeap *int_fib_heap_create(void) {
    return calloc(1, sizeof(IntFibonacciHeap));
}

void int_fib_heap_free(IntFibonacciHeap *heap) {
    if (heap == NULL) return;
    free_ring(heap->min);
    free(heap);
}

size_t int_fib_heap_size(const IntFibonacciHeap *heap) {
    return heap == NULL ? 0 : heap->size;
}

IntFibonacciNode *int_fib_heap_insert(
    IntFibonacciHeap *heap,
    int key
) {
    if (heap == NULL || heap->size == SIZE_MAX) return NULL;

    IntFibonacciNode *node = make_node(key);
    if (node == NULL) return NULL;

    add_root(heap, node);
    ++heap->size;
    return node;
}

bool int_fib_heap_peek_min(
    const IntFibonacciHeap *heap,
    int *out_key
) {
    if (heap == NULL || heap->min == NULL || out_key == NULL) {
        return false;
    }

    *out_key = heap->min->key;
    return true;
}

bool int_fib_heap_meld(
    IntFibonacciHeap *destination,
    IntFibonacciHeap *source
) {
    if (destination == NULL || source == NULL) return false;
    if (destination == source) return false;
    if (destination->size > SIZE_MAX - source->size) return false;

    if (source->min == NULL) return true;

    if (destination->min == NULL) {
        destination->min = source->min;
    } else {
        IntFibonacciNode *source_min = source->min;
        concatenate_lists(destination->min, source->min);

        if (source_min->key < destination->min->key) {
            destination->min = source_min;
        }
    }

    destination->size += source->size;
    source->min = NULL;
    source->size = 0;

    return true;
}

static bool ensure_degree_capacity(
    IntFibonacciNode ***table,
    size_t *capacity,
    size_t needed
) {
    if (needed < *capacity) return true;

    size_t next_capacity = *capacity == 0 ? 16 : *capacity;

    while (needed >= next_capacity) {
        if (next_capacity > SIZE_MAX / 2) return false;
        next_capacity *= 2;
    }

    if (next_capacity > SIZE_MAX / sizeof **table) return false;

    IntFibonacciNode **next = realloc(
        *table,
        next_capacity * sizeof *next
    );
    if (next == NULL) return false;

    for (size_t i = *capacity; i < next_capacity; ++i) {
        next[i] = NULL;
    }

    *table = next;
    *capacity = next_capacity;
    return true;
}

static void link_under(
    IntFibonacciNode *child,
    IntFibonacciNode *parent
) {
    child->parent = parent;
    child->mark = false;

    if (parent->child == NULL) {
        child->left = child;
        child->right = child;
        parent->child = child;
    } else {
        list_insert_after(parent->child, child);
    }

    ++parent->degree;
}

static bool consolidate(IntFibonacciHeap *heap) {
    if (heap->min == NULL) return true;

    size_t capacity = 16;
    IntFibonacciNode **table = calloc(capacity, sizeof *table);
    if (table == NULL) return false;

    IntFibonacciNode *remaining = heap->min;
    heap->min = NULL;

    while (remaining != NULL) {
        IntFibonacciNode *x = remaining;

        if (x->right == x) {
            remaining = NULL;
        } else {
            remaining = x->right;
            list_remove(x);
        }

        size_t degree = x->degree;

        for (;;) {
            if (!ensure_degree_capacity(&table, &capacity, degree)) {
                add_root(heap, x);

                for (size_t i = 0; i < capacity; ++i) {
                    if (table[i] != NULL) {
                        add_root(heap, table[i]);
                        table[i] = NULL;
                    }
                }

                while (remaining != NULL) {
                    IntFibonacciNode *next = NULL;

                    if (remaining->right != remaining) {
                        next = remaining->right;
                    }

                    list_remove(remaining);
                    add_root(heap, remaining);
                    remaining = next;
                }

                free(table);
                return false;
            }

            IntFibonacciNode *other = table[degree];

            if (other == NULL) {
                table[degree] = x;
                break;
            }

            table[degree] = NULL;

            if (other->key < x->key) {
                IntFibonacciNode *tmp = x;
                x = other;
                other = tmp;
            }

            link_under(other, x);
            degree = x->degree;
        }
    }

    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] != NULL) {
            add_root(heap, table[i]);
        }
    }

    free(table);
    return true;
}

static bool extract_min_node(
    IntFibonacciHeap *heap,
    IntFibonacciNode *target,
    int *out_key
) {
    if (heap == NULL || target == NULL || heap->size == 0) return false;

    if (target->child != NULL) {
        IntFibonacciNode *child = target->child;
        const size_t degree = target->degree;

        for (size_t i = 0; i < degree; ++i) {
            IntFibonacciNode *next = child->right;
            list_remove(child);
            add_root(heap, child);
            child = next;
        }

        target->child = NULL;
        target->degree = 0;
    }

    if (target->right == target) {
        heap->min = NULL;
    } else {
        IntFibonacciNode *next_root = target->right;
        list_remove(target);
        heap->min = next_root;
    }

    const int key = target->key;
    --heap->size;

    if (heap->min != NULL) {
        (void)consolidate(heap);
    }

    if (out_key != NULL) *out_key = key;

    free(target);
    return true;
}

bool int_fib_heap_extract_min(
    IntFibonacciHeap *heap,
    int *out_key
) {
    if (heap == NULL || heap->min == NULL) return false;
    return extract_min_node(heap, heap->min, out_key);
}

static void cut(
    IntFibonacciHeap *heap,
    IntFibonacciNode *node,
    IntFibonacciNode *parent
) {
    if (node->right == node) {
        parent->child = NULL;
    } else {
        if (parent->child == node) {
            parent->child = node->right;
        }
        list_remove(node);
    }

    --parent->degree;
    add_root(heap, node);
}

static void cascading_cut(
    IntFibonacciHeap *heap,
    IntFibonacciNode *node
) {
    IntFibonacciNode *parent = node->parent;

    if (parent == NULL) return;

    if (!node->mark) {
        node->mark = true;
        return;
    }

    cut(heap, node, parent);
    cascading_cut(heap, parent);
}

bool int_fib_heap_decrease_key(
    IntFibonacciHeap *heap,
    IntFibonacciNode *node,
    int new_key
) {
    if (heap == NULL || node == NULL) return false;
    if (new_key > node->key) return false;

    node->key = new_key;
    IntFibonacciNode *parent = node->parent;

    if (parent != NULL && node->key < parent->key) {
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }

    if (heap->min == NULL || node->key < heap->min->key) {
        heap->min = node;
    }

    return true;
}

bool int_fib_heap_delete_handle(
    IntFibonacciHeap *heap,
    IntFibonacciNode *node
) {
    if (heap == NULL || node == NULL || heap->size == 0) return false;

    if (node->parent != NULL) {
        IntFibonacciNode *parent = node->parent;
        cut(heap, node, parent);
        cascading_cut(heap, parent);
    }

    heap->min = node;
    return extract_min_node(heap, node, NULL);
}

int int_fib_node_key(const IntFibonacciNode *node) {
    return node == NULL ? 0 : node->key;
}

static bool validate_node(
    const IntFibonacciNode *node,
    const IntFibonacciNode *expected_parent,
    size_t limit,
    size_t *count
) {
    if (node == NULL) return false;

    if (node->parent != expected_parent) return false;
    if (node->left == NULL || node->right == NULL) return false;
    if (node->left->right != node) return false;
    if (node->right->left != node) return false;
    if (expected_parent == NULL && node->mark) return false;

    if (*count >= limit) return false;
    ++*count;

    if (node->degree == 0) {
        return node->child == NULL;
    }

    if (node->child == NULL) return false;

    const IntFibonacciNode *child = node->child;

    for (size_t i = 0; i < node->degree; ++i) {
        if (child == NULL) return false;
        if (child->parent != node) return false;
        if (node->key > child->key) return false;

        if (!validate_node(child, node, limit, count)) {
            return false;
        }

        child = child->right;
    }

    return child == node->child;
}

bool int_fib_heap_validate(
    const IntFibonacciHeap *heap
) {
    if (heap == NULL) return false;

    if (heap->size == 0) {
        return heap->min == NULL;
    }

    if (heap->min == NULL) return false;

    const IntFibonacciNode *root = heap->min;
    size_t count = 0;
    int minimum = heap->min->key;
    size_t root_steps = 0;

    do {
        if (root_steps++ >= heap->size) return false;
        if (root->parent != NULL) return false;
        if (root->mark) return false;

        if (root->key < minimum) minimum = root->key;

        if (!validate_node(root, NULL, heap->size, &count)) {
            return false;
        }

        root = root->right;
    } while (root != heap->min);

    return count == heap->size && heap->min->key == minimum;
}
