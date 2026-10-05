#include "int_avl.h"

#include <limits.h>
#include <stdlib.h>

typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

struct IntAVL {
    AVLNode *root;
    size_t size;
};

static int height_of(const AVLNode *node) {
    return node == NULL ? 0 : node->height;
}

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static void update_height(AVLNode *node) {
    node->height = 1 + max_int(height_of(node->left), height_of(node->right));
}

static int balance_factor(const AVLNode *node) {
    return node == NULL ? 0 : height_of(node->left) - height_of(node->right);
}

static AVLNode *rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *middle = x->right;

    x->right = y;
    y->left = middle;

    update_height(y);
    update_height(x);

    return x;
}

static AVLNode *rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *middle = y->left;

    y->left = x;
    x->right = middle;

    update_height(x);
    update_height(y);

    return y;
}

static AVLNode *rebalance(AVLNode *node) {
    if (node == NULL) return NULL;

    update_height(node);
    const int bf = balance_factor(node);

    if (bf > 1) {
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }

    if (bf < -1) {
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }

    return node;
}

static AVLNode *make_node(int key) {
    AVLNode *node = calloc(1, sizeof *node);
    if (node == NULL) return NULL;

    node->key = key;
    node->height = 1;
    return node;
}

IntAVL *int_avl_create(void) {
    return calloc(1, sizeof(IntAVL));
}

static void free_subtree(AVLNode *node) {
    if (node == NULL) return;
    free_subtree(node->left);
    free_subtree(node->right);
    free(node);
}

void int_avl_free(IntAVL *tree) {
    if (tree == NULL) return;
    free_subtree(tree->root);
    free(tree);
}

size_t int_avl_size(const IntAVL *tree) {
    return tree == NULL ? 0 : tree->size;
}

bool int_avl_contains(const IntAVL *tree, int key) {
    if (tree == NULL) return false;

    AVLNode *node = tree->root;

    while (node != NULL) {
        if (key == node->key) return true;
        node = key < node->key ? node->left : node->right;
    }

    return false;
}

static AVLNode *insert_rec(AVLNode *node, int key, int *status) {
    if (node == NULL) {
        AVLNode *created = make_node(key);

        if (created == NULL) {
            *status = -1;
            return NULL;
        }

        *status = 1;
        return created;
    }

    if (key == node->key) {
        *status = 0;
        return node;
    }

    if (key < node->key) {
        AVLNode *new_left = insert_rec(node->left, key, status);
        if (*status == -1) return node;
        node->left = new_left;
    } else {
        AVLNode *new_right = insert_rec(node->right, key, status);
        if (*status == -1) return node;
        node->right = new_right;
    }

    return rebalance(node);
}

bool int_avl_insert(IntAVL *tree, int key) {
    if (tree == NULL) return false;

    int status = 0;
    AVLNode *new_root = insert_rec(tree->root, key, &status);

    if (status == -1) return false;
    if (status == 0) return false;

    tree->root = new_root;
    ++tree->size;
    return true;
}

static AVLNode *minimum_node(AVLNode *node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

static AVLNode *remove_rec(AVLNode *node, int key, bool *removed) {
    if (node == NULL) return NULL;

    if (key < node->key) {
        node->left = remove_rec(node->left, key, removed);
    } else if (key > node->key) {
        node->right = remove_rec(node->right, key, removed);
    } else {
        *removed = true;

        if (node->left == NULL || node->right == NULL) {
            AVLNode *child = node->left != NULL ? node->left : node->right;
            free(node);
            return child;
        }

        AVLNode *successor = minimum_node(node->right);
        node->key = successor->key;

        bool successor_removed = false;
        node->right = remove_rec(node->right, successor->key, &successor_removed);
    }

    return rebalance(node);
}

bool int_avl_remove(IntAVL *tree, int key) {
    if (tree == NULL) return false;

    bool removed = false;
    tree->root = remove_rec(tree->root, key, &removed);

    if (removed) {
        --tree->size;
    }

    return removed;
}

bool int_avl_height(const IntAVL *tree, size_t *out_edge_height) {
    if (tree == NULL || tree->root == NULL || out_edge_height == NULL) return false;

    *out_edge_height = (size_t)(tree->root->height - 1);
    return true;
}

static void inorder_node(const AVLNode *node, int *output, size_t *index) {
    if (node == NULL) return;

    inorder_node(node->left, output, index);
    output[(*index)++] = node->key;
    inorder_node(node->right, output, index);
}

bool int_avl_inorder(
    const IntAVL *tree,
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
    const AVLNode *node,
    bool has_low,
    int low,
    bool has_high,
    int high,
    size_t limit,
    size_t *count,
    int *out_height
) {
    if (node == NULL) {
        *out_height = 0;
        return true;
    }

    if (has_low && node->key <= low) return false;
    if (has_high && node->key >= high) return false;

    ++*count;
    if (*count > limit) return false;

    int left_height = 0;
    int right_height = 0;

    if (!validate_node(
            node->left,
            has_low,
            low,
            true,
            node->key,
            limit,
            count,
            &left_height
        )) {
        return false;
    }

    if (!validate_node(
            node->right,
            true,
            node->key,
            has_high,
            high,
            limit,
            count,
            &right_height
        )) {
        return false;
    }

    const int expected_height = 1 + max_int(left_height, right_height);
    const int bf = left_height - right_height;

    if (node->height != expected_height) return false;
    if (bf < -1 || bf > 1) return false;

    *out_height = expected_height;
    return true;
}

bool int_avl_validate(const IntAVL *tree) {
    if (tree == NULL) return false;

    if (tree->size == 0) return tree->root == NULL;
    if (tree->root == NULL) return false;

    size_t count = 0;
    int height = 0;

    if (!validate_node(
            tree->root,
            false,
            INT_MIN,
            false,
            INT_MAX,
            tree->size,
            &count,
            &height
        )) {
        return false;
    }

    return count == tree->size && height == tree->root->height;
}
