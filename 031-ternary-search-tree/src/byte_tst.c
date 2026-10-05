#include "byte_tst.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct TSTNode {
    unsigned char symbol;
    bool terminal;
    struct TSTNode *low;
    struct TSTNode *equal;
    struct TSTNode *high;
} TSTNode;

struct ByteTST {
    TSTNode *root;
    size_t size;
    bool empty_terminal;
};

static TSTNode *make_node(unsigned char symbol) {
    TSTNode *node = calloc(1, sizeof *node);
    if (node != NULL) node->symbol = symbol;
    return node;
}

static void free_node(TSTNode *node) {
    if (node == NULL) return;
    free_node(node->low);
    free_node(node->equal);
    free_node(node->high);
    free(node);
}

ByteTST *byte_tst_create(void) {
    return calloc(1, sizeof(ByteTST));
}

void byte_tst_free(ByteTST *tree) {
    if (tree == NULL) return;
    free_node(tree->root);
    free(tree);
}

size_t byte_tst_size(const ByteTST *tree) {
    return tree == NULL ? 0 : tree->size;
}

static TSTNode *insert_rec(
    TSTNode *node,
    const unsigned char *bytes,
    size_t index,
    size_t length,
    bool *inserted,
    bool *oom
) {
    if (node == NULL) {
        node = make_node(bytes[index]);
        if (node == NULL) {
            *oom = true;
            return NULL;
        }
    }

    if (bytes[index] < node->symbol) {
        TSTNode *next = insert_rec(
            node->low, bytes, index, length, inserted, oom
        );
        if (*oom && node->low == NULL) return node;
        node->low = next;
    } else if (bytes[index] > node->symbol) {
        TSTNode *next = insert_rec(
            node->high, bytes, index, length, inserted, oom
        );
        if (*oom && node->high == NULL) return node;
        node->high = next;
    } else if (index + 1 == length) {
        if (!node->terminal) {
            node->terminal = true;
            *inserted = true;
        }
    } else {
        TSTNode *next = insert_rec(
            node->equal, bytes, index + 1, length, inserted, oom
        );
        if (*oom && node->equal == NULL) return node;
        node->equal = next;
    }

    return node;
}

bool byte_tst_insert(
    ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;
    if (length > 0 && bytes == NULL) return false;

    if (length == 0) {
        if (tree->empty_terminal) return false;
        tree->empty_terminal = true;
        ++tree->size;
        return true;
    }

    bool inserted = false;
    bool oom = false;

    tree->root = insert_rec(
        tree->root, bytes, 0, length, &inserted, &oom
    );

    if (oom) return false;

    if (inserted) ++tree->size;
    return inserted;
}

static const TSTNode *find_terminal_node(
    const ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL || length == 0 || bytes == NULL) return NULL;

    const TSTNode *node = tree->root;
    size_t index = 0;

    while (node != NULL) {
        if (bytes[index] < node->symbol) {
            node = node->low;
        } else if (bytes[index] > node->symbol) {
            node = node->high;
        } else {
            if (index + 1 == length) return node;
            ++index;
            node = node->equal;
        }
    }

    return NULL;
}

bool byte_tst_contains(
    const ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;

    if (length == 0) {
        return bytes == NULL ? tree->empty_terminal
                             : tree->empty_terminal;
    }

    if (bytes == NULL) return false;

    const TSTNode *node = find_terminal_node(tree, bytes, length);
    return node != NULL && node->terminal;
}

bool byte_tst_has_prefix(
    const ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;
    if (length == 0) return true;
    if (bytes == NULL) return false;

    return find_terminal_node(tree, bytes, length) != NULL;
}

static size_t count_all(const TSTNode *node) {
    if (node == NULL) return 0;

    return count_all(node->low) +
           (node->terminal ? 1U : 0U) +
           count_all(node->equal) +
           count_all(node->high);
}

size_t byte_tst_count_prefix(
    const ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return 0;

    if (length == 0) return tree->size;
    if (bytes == NULL) return 0;

    const TSTNode *node = find_terminal_node(tree, bytes, length);
    if (node == NULL) return 0;

    return (node->terminal ? 1U : 0U) + count_all(node->equal);
}

static bool node_empty(const TSTNode *node) {
    return node != NULL &&
           !node->terminal &&
           node->low == NULL &&
           node->equal == NULL &&
           node->high == NULL;
}

static TSTNode *remove_rec(
    TSTNode *node,
    const unsigned char *bytes,
    size_t index,
    size_t length,
    bool *removed
) {
    if (node == NULL) return NULL;

    if (bytes[index] < node->symbol) {
        node->low = remove_rec(
            node->low, bytes, index, length, removed
        );
    } else if (bytes[index] > node->symbol) {
        node->high = remove_rec(
            node->high, bytes, index, length, removed
        );
    } else if (index + 1 == length) {
        if (node->terminal) {
            node->terminal = false;
            *removed = true;
        }
    } else {
        node->equal = remove_rec(
            node->equal, bytes, index + 1, length, removed
        );
    }

    if (*removed && node_empty(node)) {
        free(node);
        return NULL;
    }

    return node;
}

bool byte_tst_remove(
    ByteTST *tree,
    const unsigned char *bytes,
    size_t length
) {
    if (tree == NULL) return false;

    if (length == 0) {
        if (!tree->empty_terminal) return false;
        tree->empty_terminal = false;
        --tree->size;
        return true;
    }

    if (bytes == NULL) return false;

    bool removed = false;
    tree->root = remove_rec(
        tree->root, bytes, 0, length, &removed
    );

    if (removed) --tree->size;
    return removed;
}

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t capacity;
    byte_tst_visit_fn visitor;
    void *context;
    bool ok;
} VisitState;

static bool ensure_buffer(VisitState *state, size_t needed) {
    if (needed <= state->capacity) return true;

    size_t capacity = state->capacity == 0 ? 16 : state->capacity;

    while (capacity < needed) {
        if (capacity > SIZE_MAX / 2) {
            capacity = needed;
            break;
        }
        capacity *= 2;
    }

    unsigned char *next = realloc(state->buffer, capacity);
    if (next == NULL) return false;

    state->buffer = next;
    state->capacity = capacity;
    return true;
}

static void visit_nodes(const TSTNode *node, VisitState *state) {
    if (node == NULL || !state->ok) return;

    visit_nodes(node->low, state);
    if (!state->ok) return;

    if (!ensure_buffer(state, state->length + 1)) {
        state->ok = false;
        return;
    }

    state->buffer[state->length++] = node->symbol;

    if (node->terminal) {
        if (!state->visitor(
                state->buffer, state->length, state->context
            )) {
            state->ok = false;
            --state->length;
            return;
        }
    }

    visit_nodes(node->equal, state);
    --state->length;

    visit_nodes(node->high, state);
}

bool byte_tst_visit_prefix(
    const ByteTST *tree,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_tst_visit_fn visitor,
    void *context
) {
    if (tree == NULL || visitor == NULL) return false;
    if (prefix_length > 0 && prefix == NULL) return false;

    VisitState state = {
        .visitor = visitor,
        .context = context,
        .ok = true
    };

    if (prefix_length == 0) {
        if (tree->empty_terminal) {
            if (!visitor(NULL, 0, context)) return false;
        }

        visit_nodes(tree->root, &state);
        free(state.buffer);
        return state.ok;
    }

    const TSTNode *node = find_terminal_node(
        tree, prefix, prefix_length
    );

    if (node == NULL) return true;

    if (!ensure_buffer(&state, prefix_length)) return false;

    memcpy(state.buffer, prefix, prefix_length);
    state.length = prefix_length;

    if (node->terminal) {
        if (!visitor(state.buffer, state.length, context)) {
            free(state.buffer);
            return false;
        }
    }

    visit_nodes(node->equal, &state);
    free(state.buffer);
    return state.ok;
}

static bool validate_level(
    const TSTNode *node,
    bool has_low,
    unsigned char low,
    bool has_high,
    unsigned char high,
    size_t *terminal_count
) {
    if (node == NULL) return true;

    if (has_low && node->symbol <= low) return false;
    if (has_high && node->symbol >= high) return false;

    if (node->terminal) ++*terminal_count;

    if (!validate_level(
            node->low,
            has_low,
            low,
            true,
            node->symbol,
            terminal_count
        )) {
        return false;
    }

    if (!validate_level(
            node->equal,
            false,
            0,
            false,
            0,
            terminal_count
        )) {
        return false;
    }

    return validate_level(
        node->high,
        true,
        node->symbol,
        has_high,
        high,
        terminal_count
    );
}

bool byte_tst_validate(const ByteTST *tree) {
    if (tree == NULL) return false;

    size_t terminals = tree->empty_terminal ? 1U : 0U;

    if (!validate_level(
            tree->root, false, 0, false, 0, &terminals
        )) {
        return false;
    }

    return terminals == tree->size;
}
