#include "persistent_int_set.h"
#include <stdint.h>
#include <stdlib.h>

enum { PSET_BLOCK_CAPACITY = 256 };

struct PSetNode {
    int key;
    size_t size;
    const struct PSetNode *left;
    const struct PSetNode *right;
};

typedef struct PSetBlock {
    struct PSetBlock *next;
    size_t used;
    struct PSetNode nodes[PSET_BLOCK_CAPACITY];
} PSetBlock;

struct PSetArena {
    PSetBlock *head;
    size_t node_count;
    size_t block_count;
};

typedef struct {
    PSetBlock *head;
    size_t head_used;
    size_t node_count;
    size_t block_count;
} ArenaMark;

static ArenaMark arena_mark(const PSetArena *a) {
    ArenaMark m = {a->head, a->head ? a->head->used : 0, a->node_count, a->block_count};
    return m;
}

static void arena_rollback(PSetArena *a, ArenaMark m) {
    while (a->head != m.head) {
        PSetBlock *dead = a->head;
        a->head = dead->next;
        free(dead);
    }
    if (a->head) a->head->used = m.head_used;
    a->node_count = m.node_count;
    a->block_count = m.block_count;
}

static size_t node_size(const PSetNode *n) { return n ? n->size : 0; }

static PSetNode *arena_node(PSetArena *a, int key, const PSetNode *left, const PSetNode *right) {
    size_t ls = node_size(left), rs = node_size(right);
    if (ls > SIZE_MAX - rs - 1U) return NULL;
    if (!a->head || a->head->used == PSET_BLOCK_CAPACITY) {
        PSetBlock *b = calloc(1, sizeof(*b));
        if (!b) return NULL;
        b->next = a->head;
        a->head = b;
        ++a->block_count;
    }
    PSetNode *n = &a->head->nodes[a->head->used++];
    n->key = key;
    n->size = 1U + ls + rs;
    n->left = left;
    n->right = right;
    ++a->node_count;
    return n;
}

PSetArena *pset_arena_create(void) { return calloc(1, sizeof(PSetArena)); }

void pset_arena_free(PSetArena *a) {
    if (!a) return;
    while (a->head) {
        PSetBlock *next = a->head->next;
        free(a->head);
        a->head = next;
    }
    free(a);
}

size_t pset_arena_allocated_nodes(const PSetArena *a) { return a ? a->node_count : 0; }
size_t pset_arena_allocated_blocks(const PSetArena *a) { return a ? a->block_count : 0; }

static bool insert_rec(PSetArena *a, const PSetNode *root, int key,
                       const PSetNode **out, bool *inserted) {
    if (!root) {
        PSetNode *n = arena_node(a, key, NULL, NULL);
        if (!n) return false;
        *out = n; *inserted = true; return true;
    }
    if (key == root->key) {
        *out = root; *inserted = false; return true;
    }

    const PSetNode *child = NULL;
    bool did = false;
    if (key < root->key) {
        if (!insert_rec(a, root->left, key, &child, &did)) return false;
        if (!did) { *out = root; *inserted = false; return true; }
        PSetNode *copy = arena_node(a, root->key, child, root->right);
        if (!copy) return false;
        *out = copy;
    } else {
        if (!insert_rec(a, root->right, key, &child, &did)) return false;
        if (!did) { *out = root; *inserted = false; return true; }
        PSetNode *copy = arena_node(a, root->key, root->left, child);
        if (!copy) return false;
        *out = copy;
    }
    *inserted = true;
    return true;
}

bool pset_insert(PSetArena *a, const PSetNode *root, int key,
                 const PSetNode **out_root, bool *out_inserted) {
    if (!a || !out_root) return false;
    ArenaMark m = arena_mark(a);
    bool inserted = false;
    const PSetNode *next = NULL;
    if (!insert_rec(a, root, key, &next, &inserted)) {
        arena_rollback(a, m);
        return false;
    }
    *out_root = next;
    if (out_inserted) *out_inserted = inserted;
    return true;
}

static const PSetNode *min_node(const PSetNode *n) {
    while (n && n->left) n = n->left;
    return n;
}

static bool erase_rec(PSetArena *a, const PSetNode *root, int key,
                      const PSetNode **out, bool *erased) {
    if (!root) { *out = NULL; *erased = false; return true; }

    if (key < root->key) {
        const PSetNode *left = NULL; bool did = false;
        if (!erase_rec(a, root->left, key, &left, &did)) return false;
        if (!did) { *out = root; *erased = false; return true; }
        PSetNode *copy = arena_node(a, root->key, left, root->right);
        if (!copy) return false;
        *out = copy; *erased = true; return true;
    }
    if (key > root->key) {
        const PSetNode *right = NULL; bool did = false;
        if (!erase_rec(a, root->right, key, &right, &did)) return false;
        if (!did) { *out = root; *erased = false; return true; }
        PSetNode *copy = arena_node(a, root->key, root->left, right);
        if (!copy) return false;
        *out = copy; *erased = true; return true;
    }

    if (!root->left) { *out = root->right; *erased = true; return true; }
    if (!root->right) { *out = root->left; *erased = true; return true; }

    const PSetNode *succ = min_node(root->right);
    const PSetNode *new_right = NULL;
    bool did = false;
    if (!erase_rec(a, root->right, succ->key, &new_right, &did) || !did) return false;
    PSetNode *copy = arena_node(a, succ->key, root->left, new_right);
    if (!copy) return false;
    *out = copy; *erased = true; return true;
}

bool pset_erase(PSetArena *a, const PSetNode *root, int key,
                const PSetNode **out_root, bool *out_erased) {
    if (!a || !out_root) return false;
    ArenaMark m = arena_mark(a);
    bool erased = false;
    const PSetNode *next = NULL;
    if (!erase_rec(a, root, key, &next, &erased)) {
        arena_rollback(a, m);
        return false;
    }
    *out_root = next;
    if (out_erased) *out_erased = erased;
    return true;
}

bool pset_contains(const PSetNode *root, int key) {
    const PSetNode *n = root;
    while (n) {
        if (key == n->key) return true;
        n = key < n->key ? n->left : n->right;
    }
    return false;
}

size_t pset_size(const PSetNode *root) { return node_size(root); }

size_t pset_height(const PSetNode *root) {
    if (!root) return 0;
    size_t lh = pset_height(root->left), rh = pset_height(root->right);
    return 1U + (lh > rh ? lh : rh);
}

static bool validate_rec(const PSetNode *n, int64_t lo, int64_t hi, size_t *out_size) {
    if (!n) { *out_size = 0; return true; }
    int64_t key = n->key;
    if (key <= lo || key >= hi) return false;
    size_t ls = 0, rs = 0;
    if (!validate_rec(n->left, lo, key, &ls) || !validate_rec(n->right, key, hi, &rs)) return false;
    if (ls > SIZE_MAX - rs - 1U) return false;
    size_t expected = 1U + ls + rs;
    if (n->size != expected) return false;
    *out_size = expected;
    return true;
}

bool pset_validate(const PSetNode *root) {
    size_t size = 0;
    return validate_rec(root, INT64_MIN, INT64_MAX, &size);
}
