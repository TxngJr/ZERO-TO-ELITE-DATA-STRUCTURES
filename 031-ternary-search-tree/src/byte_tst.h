#ifndef BYTE_TST_H
#define BYTE_TST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct ByteTST ByteTST;

typedef bool (*byte_tst_visit_fn)(
    const unsigned char *bytes,
    size_t length,
    void *context
);

ByteTST *byte_tst_create(void);
void byte_tst_free(ByteTST *tree);

size_t byte_tst_size(const ByteTST *tree);

bool byte_tst_insert(ByteTST *tree, const unsigned char *bytes, size_t length);
bool byte_tst_contains(const ByteTST *tree, const unsigned char *bytes, size_t length);
bool byte_tst_has_prefix(const ByteTST *tree, const unsigned char *bytes, size_t length);
size_t byte_tst_count_prefix(const ByteTST *tree, const unsigned char *bytes, size_t length);
bool byte_tst_remove(ByteTST *tree, const unsigned char *bytes, size_t length);

bool byte_tst_visit_prefix(
    const ByteTST *tree,
    const unsigned char *prefix,
    size_t prefix_length,
    byte_tst_visit_fn visitor,
    void *context
);

bool byte_tst_validate(const ByteTST *tree);

#endif
