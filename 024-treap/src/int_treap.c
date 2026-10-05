#include "int_treap.h"

#include <limits.h>
#include <stdlib.h>

typedef struct TreapNode {
    int key;
    uint32_t priority;
    struct TreapNode *parent;
    struct TreapNode *left;
    struct TreapNode *right;
} TreapNode;

struct IntTreap {
    TreapNode *root;
    size_t size;
    uint64_t rng_state;
};

static bool outranks(const TreapNode *a, const TreapNode *b) {
    if (a->priority != b->priority) {
        return a->priority < b->priority;
    }
    return a->key < b->key;
}

static TreapNode *make_node(int key, uint32_t priority) {
    TreapNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->key = key;
    node->priority = priority;
    return node;
}

static void free_subtree(TreapNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

IntTreap *int_treap_create(void) {
    IntTreap *tree = calloc(1, sizeof *tree);
    if (tree != NULL) {
        tree->rng_state = UINT64_C(0x9e3779b97f4a7c15);
    }
    return tree;
}

void int_treap_free(IntTreap *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t int_treap_size(const IntTreap *tree) {
    return tree == NULL ? 0 : tree->size;
}

static TreapNode *find_node(const IntTreap *tree, int key) {
    if (tree == NULL) return NULL;

    TreapNode *node = tree->root;

    while (node != NULL) {
        if (key == node->key) return node;
        node = key < node->key ? node->left : node->right;
    }

    return NULL;
}

bool int_treap_contains(const IntTreap *tree, int key) {
    return find_node(tree, key) != NULL;
}

static void rotate_left(IntTreap *tree, TreapNode *x) {
    TreapNode *y = x->right;
    x->right = y->left;

    if (y->left != NULL) y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void rotate_right(IntTreap *tree, TreapNode *y) {
    TreapNode *x = y->left;
    y->left = x->right;

    if (x->right != NULL) x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL) tree->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    x->right = y;
    y->parent = x;
}

bool int_treap_insert_with_priority(
    IntTreap *tree,
    int key,
    uint32_t priority
) {
    if (tree == NULL) return false;

    TreapNode *parent = NULL;
    TreapNode *cursor = tree->root;

    while (cursor != NULL) {
        parent = cursor;

        if (key == cursor->key) return false;

        cursor = key < cursor->key ? cursor->left : cursor->right;
    }

    TreapNode *node = make_node(key, priority);
    if (node == NULL) return false;

    node->parent = parent;

    if (parent == NULL) tree->root = node;
    else if (key < parent->key) parent->left = node;
    else parent->right = node;

    ++tree->size;

    while (node->parent != NULL && outranks(node, node->parent)) {
        if (node == node->parent->left) {
            rotate_right(tree, node->parent);
        } else {
            rotate_left(tree, node->parent);
        }
    }

    return true;
}

static uint32_t next_priority(IntTreap *tree) {
    uint64_t x = tree->rng_state;

    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;

    tree->rng_state = x;

    x *= UINT64_C(2685821657736338717);
    return (uint32_t)(x >> 32);
}

bool int_treap_insert(IntTreap *tree, int key) {
    if (tree == NULL) return false;
    return int_treap_insert_with_priority(tree, key, next_priority(tree));
}

static TreapNode *merge_nodes(
    TreapNode *left,
    TreapNode *right,
    TreapNode *parent
) {
    if (left == NULL) {
        if (right != NULL) right->parent = parent;
        return right;
    }

    if (right == NULL) {
        left->parent = parent;
        return left;
    }

    if (outranks(left, right)) {
        left->parent = parent;
        left->right = merge_nodes(left->right, right, left);
        return left;
    }

    right->parent = parent;
    right->left = merge_nodes(left, right->left, right);
    return right;
}

static TreapNode *remove_rec(
    TreapNode *node,
    int key,
    bool *removed
) {
    if (node == NULL) return NULL;

    if (key < node->key) {
        node->left = remove_rec(node->left, key, removed);
        if (node->left != NULL) node->left->parent = node;
        return node;
    }

    if (key > node->key) {
        node->right = remove_rec(node->right, key, removed);
        if (node->right != NULL) node->right->parent = node;
        return node;
    }

    *removed = true;

    TreapNode *parent = node->parent;
    TreapNode *merged = merge_nodes(node->left, node->right, parent);

    free(node);
    return merged;
}

bool int_treap_remove(IntTreap *tree, int key) {
    if (tree == NULL) return false;

    bool removed = false;
    tree->root = remove_rec(tree->root, key, &removed);

    if (tree->root != NULL) tree->root->parent = NULL;

    if (removed) --tree->size;
    return removed;
}

bool int_treap_root(
    const IntTreap *tree,
    int *out_key,
    uint32_t *out_priority
) {
    if (tree == NULL || tree->root == NULL) return false;

    if (out_key != NULL) *out_key = tree->root->key;
    if (out_priority != NULL) *out_priority = tree->root->priority;

    return true;
}

static size_t node_height(const TreapNode *node) {
    if (node == NULL) return 0;

    const size_t left =
        node->left == NULL ? 0 : node_height(node->left) + 1;
    const size_t right =
        node->right == NULL ? 0 : node_height(node->right) + 1;

    return left > right ? left : right;
}

bool int_treap_height(
    const IntTreap *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL || out_edge_height == NULL) {
        return false;
    }

    *out_edge_height = node_height(tree->root);
    return true;
}

static void inorder_node(
    const TreapNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;

    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool int_treap_inorder(
    const IntTreap *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) return false;

    if (tree->size > 0 &&
        (output == NULL || capacity < tree->size)) {
        return false;
    }

    *out_written = 0;
    inorder_node(tree->root, output, out_written);
    return true;
}

static bool validate_node(
    const TreapNode *node,
    const TreapNode *parent,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t limit,
    size_t *count
) {
    if (node == NULL) return true;

    if (node->parent != parent) return false;
    if (has_low && node->key <= low) return false;
    if (has_high && node->key >= high) return false;

    if (node->left != NULL && outranks(node->left, node)) return false;
    if (node->right != NULL && outranks(node->right, node)) return false;

    ++*count;
    if (*count > limit) return false;

    return validate_node(
               node->left,
               node,
               has_low,
               low,
               true,
               node->key,
               limit,
               count
           ) &&
           validate_node(
               node->right,
               node,
               true,
               node->key,
               has_high,
               high,
               limit,
               count
           );
}

bool int_treap_validate(const IntTreap *tree) {
    if (tree == NULL) return false;

    if (tree->size == 0) return tree->root == NULL;
    if (tree->root == NULL || tree->root->parent != NULL) return false;

    size_t count = 0;

    if (!validate_node(
            tree->root,
            NULL,
            false,
            INT_MIN,
            false,
            INT_MAX,
            tree->size,
            &count
        )) {
        return false;
    }

    return count == tree->size;
}
