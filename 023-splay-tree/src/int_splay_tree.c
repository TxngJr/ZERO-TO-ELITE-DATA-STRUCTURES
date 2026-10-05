#include "int_splay_tree.h"

#include <limits.h>
#include <stdlib.h>

typedef struct SplayNode {
    int key;
    struct SplayNode *parent;
    struct SplayNode *left;
    struct SplayNode *right;
} SplayNode;

struct IntSplayTree {
    SplayNode *root;
    size_t size;
};

static SplayNode *make_node(int key) {
    SplayNode *node = calloc(1, sizeof *node);
    if (node != NULL) node->key = key;
    return node;
}

static void free_subtree(SplayNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

IntSplayTree *int_splay_create(void) {
    return calloc(1, sizeof(IntSplayTree));
}

void int_splay_free(IntSplayTree *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t int_splay_size(const IntSplayTree *tree) {
    return tree == NULL ? 0 : tree->size;
}

static void rotate_left(IntSplayTree *tree, SplayNode *x) {
    SplayNode *y = x->right;
    x->right = y->left;

    if (y->left != NULL) y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

static void rotate_right(IntSplayTree *tree, SplayNode *y) {
    SplayNode *x = y->left;
    y->left = x->right;

    if (x->right != NULL) x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL) tree->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;

    x->right = y;
    y->parent = x;
}

static void splay(IntSplayTree *tree, SplayNode *node) {
    while (node->parent != NULL) {
        SplayNode *parent = node->parent;
        SplayNode *grand = parent->parent;

        if (grand == NULL) {
            if (node == parent->left) rotate_right(tree, parent);
            else rotate_left(tree, parent);
        } else if (node == parent->left && parent == grand->left) {
            rotate_right(tree, grand);
            rotate_right(tree, parent);
        } else if (node == parent->right && parent == grand->right) {
            rotate_left(tree, grand);
            rotate_left(tree, parent);
        } else if (node == parent->right && parent == grand->left) {
            rotate_left(tree, parent);
            rotate_right(tree, grand);
        } else {
            rotate_right(tree, parent);
            rotate_left(tree, grand);
        }
    }
}

static SplayNode *find_or_last(
    IntSplayTree *tree,
    int key,
    bool *found
) {
    SplayNode *cursor = tree->root;
    SplayNode *last = NULL;

    while (cursor != NULL) {
        last = cursor;

        if (key == cursor->key) {
            *found = true;
            return cursor;
        }

        cursor = key < cursor->key ? cursor->left : cursor->right;
    }

    *found = false;
    return last;
}

bool int_splay_contains(IntSplayTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;

    bool found = false;
    SplayNode *node = find_or_last(tree, key, &found);

    if (node != NULL) splay(tree, node);

    return found;
}

bool int_splay_insert(IntSplayTree *tree, int key) {
    if (tree == NULL) return false;

    if (tree->root == NULL) {
        tree->root = make_node(key);
        if (tree->root == NULL) return false;
        tree->size = 1;
        return true;
    }

    SplayNode *parent = NULL;
    SplayNode *cursor = tree->root;

    while (cursor != NULL) {
        parent = cursor;

        if (key == cursor->key) {
            splay(tree, cursor);
            return false;
        }

        cursor = key < cursor->key ? cursor->left : cursor->right;
    }

    SplayNode *node = make_node(key);
    if (node == NULL) return false;

    node->parent = parent;

    if (key < parent->key) parent->left = node;
    else parent->right = node;

    ++tree->size;
    splay(tree, node);
    return true;
}

static SplayNode *maximum_node(SplayNode *node) {
    while (node != NULL && node->right != NULL) {
        node = node->right;
    }
    return node;
}

bool int_splay_remove(IntSplayTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return false;

    if (!int_splay_contains(tree, key)) return false;

    SplayNode *victim = tree->root;
    SplayNode *left = victim->left;
    SplayNode *right = victim->right;

    if (left != NULL) left->parent = NULL;
    if (right != NULL) right->parent = NULL;

    free(victim);
    --tree->size;

    if (left == NULL) {
        tree->root = right;
        return true;
    }

    tree->root = left;

    SplayNode *maximum = maximum_node(left);
    splay(tree, maximum);

    tree->root->right = right;
    if (right != NULL) right->parent = tree->root;

    return true;
}

bool int_splay_root_key(
    const IntSplayTree *tree,
    int *out_key
) {
    if (tree == NULL || tree->root == NULL || out_key == NULL) return false;

    *out_key = tree->root->key;
    return true;
}

static size_t node_height(const SplayNode *node) {
    if (node == NULL) return 0;

    const size_t left =
        node->left == NULL ? 0 : node_height(node->left) + 1;
    const size_t right =
        node->right == NULL ? 0 : node_height(node->right) + 1;

    return left > right ? left : right;
}

bool int_splay_height(
    const IntSplayTree *tree,
    size_t *out_edge_height
) {
    if (tree == NULL || tree->root == NULL || out_edge_height == NULL) {
        return false;
    }

    *out_edge_height = node_height(tree->root);
    return true;
}

static void inorder_node(
    const SplayNode *node,
    int *output,
    size_t *index
) {
    if (node == NULL) return;
    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool int_splay_inorder(
    const IntSplayTree *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) return false;
    if (tree->size > 0 && (output == NULL || capacity < tree->size)) {
        return false;
    }

    *out_written = 0;
    inorder_node(tree->root, output, out_written);
    return true;
}

static bool validate_node(
    const SplayNode *node,
    const SplayNode *parent,
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

bool int_splay_validate(const IntSplayTree *tree) {
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
