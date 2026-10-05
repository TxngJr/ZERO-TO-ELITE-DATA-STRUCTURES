#include "rot_bst.h"

#include <limits.h>
#include <stdlib.h>

struct RotBSTNode {
    int key;
    struct RotBSTNode *parent;
    struct RotBSTNode *left;
    struct RotBSTNode *right;
};

struct RotBST {
    RotBSTNode *root;
    size_t size;
};

static RotBSTNode *make_node(int key, RotBSTNode *parent) {
    RotBSTNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->key = key;
    node->parent = parent;
    return node;
}

static void free_subtree(RotBSTNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

RotBST *rot_bst_create(void) {
    return calloc(1, sizeof(RotBST));
}

void rot_bst_free(RotBST *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t rot_bst_size(const RotBST *tree) {
    return tree == NULL ? 0 : tree->size;
}

RotBSTNode *rot_bst_root(const RotBST *tree) {
    return tree == NULL ? NULL : tree->root;
}

RotBSTNode *rot_bst_find(const RotBST *tree, int key) {
    if (tree == NULL) return NULL;

    RotBSTNode *node = tree->root;

    while (node != NULL) {
        if (key == node->key) return node;
        node = key < node->key ? node->left : node->right;
    }

    return NULL;
}

bool rot_bst_insert(RotBST *tree, int key) {
    if (tree == NULL) return false;

    RotBSTNode *parent = NULL;
    RotBSTNode *node = tree->root;

    while (node != NULL) {
        parent = node;
        if (key == node->key) return false;
        node = key < node->key ? node->left : node->right;
    }

    RotBSTNode *created = make_node(key, parent);
    if (created == NULL) return false;

    if (parent == NULL) {
        tree->root = created;
    } else if (key < parent->key) {
        parent->left = created;
    } else {
        parent->right = created;
    }

    ++tree->size;
    return true;
}

static void replace_parent_link(
    RotBST *tree,
    RotBSTNode *old_root,
    RotBSTNode *new_root
) {
    RotBSTNode *parent = old_root->parent;

    new_root->parent = parent;

    if (parent == NULL) {
        tree->root = new_root;
    } else if (old_root == parent->left) {
        parent->left = new_root;
    } else {
        parent->right = new_root;
    }
}

bool rot_bst_rotate_left(RotBST *tree, int pivot_key) {
    if (tree == NULL) return false;

    RotBSTNode *x = rot_bst_find(tree, pivot_key);
    if (x == NULL || x->right == NULL) return false;

    RotBSTNode *y = x->right;
    RotBSTNode *middle = y->left;

    replace_parent_link(tree, x, y);

    y->left = x;
    x->parent = y;

    x->right = middle;
    if (middle != NULL) middle->parent = x;

    return true;
}

bool rot_bst_rotate_right(RotBST *tree, int pivot_key) {
    if (tree == NULL) return false;

    RotBSTNode *y = rot_bst_find(tree, pivot_key);
    if (y == NULL || y->left == NULL) return false;

    RotBSTNode *x = y->left;
    RotBSTNode *middle = x->right;

    replace_parent_link(tree, y, x);

    x->right = y;
    y->parent = x;

    y->left = middle;
    if (middle != NULL) middle->parent = y;

    return true;
}

int rot_bst_node_key(const RotBSTNode *node) {
    return node == NULL ? 0 : node->key;
}

static size_t height_node(const RotBSTNode *node) {
    if (node == NULL) return 0;

    const size_t left = node->left == NULL ? 0 : height_node(node->left) + 1;
    const size_t right = node->right == NULL ? 0 : height_node(node->right) + 1;

    return left > right ? left : right;
}

bool rot_bst_height(const RotBST *tree, size_t *out_height) {
    if (tree == NULL || tree->root == NULL || out_height == NULL) return false;

    *out_height = height_node(tree->root);
    return true;
}

static void inorder_node(const RotBSTNode *node, int *output, size_t *index) {
    if (node == NULL) return;

    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool rot_bst_inorder(
    const RotBST *tree,
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
    const RotBSTNode *node,
    const RotBSTNode *expected_parent,
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

bool rot_bst_validate(const RotBST *tree) {
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
