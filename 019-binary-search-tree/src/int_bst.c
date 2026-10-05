#include "int_bst.h"

#include <limits.h>
#include <stdlib.h>

struct IntBSTNode {
    int key;
    struct IntBSTNode *parent;
    struct IntBSTNode *left;
    struct IntBSTNode *right;
};

struct IntBST {
    IntBSTNode *root;
    size_t size;
};

static IntBSTNode *make_node(int key, IntBSTNode *parent) {
    IntBSTNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;
    node->key = key;
    node->parent = parent;
    return node;
}

static void free_subtree(IntBSTNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

IntBST *int_bst_create(void) {
    return calloc(1, sizeof(IntBST));
}

void int_bst_free(IntBST *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t int_bst_size(const IntBST *tree) {
    return tree == NULL ? 0 : tree->size;
}

IntBSTNode *int_bst_root(const IntBST *tree) {
    return tree == NULL ? NULL : tree->root;
}

IntBSTNode *int_bst_find(const IntBST *tree, int key) {
    if (tree == NULL) return NULL;

    IntBSTNode *node = tree->root;
    while (node != NULL) {
        if (key == node->key) return node;
        node = key < node->key ? node->left : node->right;
    }
    return NULL;
}

bool int_bst_contains(const IntBST *tree, int key) {
    return int_bst_find(tree, key) != NULL;
}

bool int_bst_insert(IntBST *tree, int key, IntBSTNode **out_node) {
    if (tree == NULL) return false;

    IntBSTNode *parent = NULL;
    IntBSTNode *node = tree->root;

    while (node != NULL) {
        parent = node;
        if (key == node->key) return false;
        node = key < node->key ? node->left : node->right;
    }

    IntBSTNode *created = make_node(key, parent);
    if (created == NULL) return false;

    if (parent == NULL) tree->root = created;
    else if (key < parent->key) parent->left = created;
    else parent->right = created;

    ++tree->size;
    if (out_node != NULL) *out_node = created;
    return true;
}

static IntBSTNode *minimum_node(IntBSTNode *node) {
    if (node == NULL) return NULL;
    while (node->left != NULL) node = node->left;
    return node;
}

static IntBSTNode *maximum_node(IntBSTNode *node) {
    if (node == NULL) return NULL;
    while (node->right != NULL) node = node->right;
    return node;
}

IntBSTNode *int_bst_minimum(const IntBST *tree) {
    return tree == NULL ? NULL : minimum_node(tree->root);
}

IntBSTNode *int_bst_maximum(const IntBST *tree) {
    return tree == NULL ? NULL : maximum_node(tree->root);
}

IntBSTNode *int_bst_successor(const IntBSTNode *node) {
    if (node == NULL) return NULL;
    if (node->right != NULL) return minimum_node(node->right);

    const IntBSTNode *current = node;
    IntBSTNode *parent = node->parent;

    while (parent != NULL && current == parent->right) {
        current = parent;
        parent = parent->parent;
    }

    return parent;
}

IntBSTNode *int_bst_predecessor(const IntBSTNode *node) {
    if (node == NULL) return NULL;
    if (node->left != NULL) return maximum_node(node->left);

    const IntBSTNode *current = node;
    IntBSTNode *parent = node->parent;

    while (parent != NULL && current == parent->left) {
        current = parent;
        parent = parent->parent;
    }

    return parent;
}

static void transplant(IntBST *tree, IntBSTNode *u, IntBSTNode *v) {
    if (u->parent == NULL) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    if (v != NULL) v->parent = u->parent;
}

bool int_bst_remove(IntBST *tree, int key) {
    if (tree == NULL) return false;

    IntBSTNode *node = int_bst_find(tree, key);
    if (node == NULL) return false;

    if (node->left == NULL) {
        transplant(tree, node, node->right);
    } else if (node->right == NULL) {
        transplant(tree, node, node->left);
    } else {
        IntBSTNode *successor = minimum_node(node->right);

        if (successor->parent != node) {
            transplant(tree, successor, successor->right);
            successor->right = node->right;
            successor->right->parent = successor;
        }

        transplant(tree, node, successor);
        successor->left = node->left;
        successor->left->parent = successor;
    }

    free(node);
    --tree->size;
    return true;
}

int int_bst_node_key(const IntBSTNode *node) {
    return node == NULL ? 0 : node->key;
}

IntBSTNode *int_bst_node_left(const IntBSTNode *node) {
    return node == NULL ? NULL : node->left;
}

IntBSTNode *int_bst_node_right(const IntBSTNode *node) {
    return node == NULL ? NULL : node->right;
}

IntBSTNode *int_bst_node_parent(const IntBSTNode *node) {
    return node == NULL ? NULL : node->parent;
}

static size_t node_height(const IntBSTNode *node) {
    if (node == NULL) return 0;
    const size_t left = node->left == NULL ? 0 : node_height(node->left) + 1;
    const size_t right = node->right == NULL ? 0 : node_height(node->right) + 1;
    return left > right ? left : right;
}

bool int_bst_height(const IntBST *tree, size_t *out_height) {
    if (tree == NULL || tree->root == NULL || out_height == NULL) return false;
    *out_height = node_height(tree->root);
    return true;
}

static void inorder_node(const IntBSTNode *node, int *output, size_t *index) {
    if (node == NULL) return;
    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool int_bst_inorder(
    const IntBST *tree,
    int *output,
    size_t capacity,
    size_t *out_written
) {
    if (tree == NULL || out_written == NULL) return false;
    if (tree->size > 0 && (output == NULL || capacity < tree->size)) return false;

    *out_written = 0;
    inorder_node(tree->root, output, out_written);
    return true;
}

static bool validate_node(
    const IntBSTNode *node,
    const IntBSTNode *expected_parent,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t limit,
    size_t *count
) {
    if (node == NULL) return true;
    if (node->parent != expected_parent) return false;
    if (has_low && node->key <= low) return false;
    if (has_high && node->key >= high) return false;

    ++*count;
    if (*count > limit) return false;

    return validate_node(node->left, node, has_low, low, true, node->key, limit, count) &&
           validate_node(node->right, node, true, node->key, has_high, high, limit, count);
}

bool int_bst_validate(const IntBST *tree) {
    if (tree == NULL) return false;
    if (tree->size == 0) return tree->root == NULL;
    if (tree->root == NULL || tree->root->parent != NULL) return false;

    size_t count = 0;
    if (!validate_node(tree->root, NULL, false, INT_MIN, false, INT_MAX, tree->size, &count)) {
        return false;
    }
    return count == tree->size;
}
