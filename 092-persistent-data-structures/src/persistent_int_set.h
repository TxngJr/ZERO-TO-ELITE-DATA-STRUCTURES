#ifndef PERSISTENT_INT_SET_H
#define PERSISTENT_INT_SET_H
#include <stdbool.h>
#include <stddef.h>

typedef struct PSetArena PSetArena;
typedef struct PSetNode PSetNode;

PSetArena *pset_arena_create(void);
void pset_arena_free(PSetArena *arena);
size_t pset_arena_allocated_nodes(const PSetArena *arena);
size_t pset_arena_allocated_blocks(const PSetArena *arena);

/* root==NULL is the empty version. Versions remain valid until arena_free. */
bool pset_insert(PSetArena *arena, const PSetNode *root, int key,
                 const PSetNode **out_root, bool *out_inserted);
bool pset_erase(PSetArena *arena, const PSetNode *root, int key,
                const PSetNode **out_root, bool *out_erased);
bool pset_contains(const PSetNode *root, int key);
size_t pset_size(const PSetNode *root);
size_t pset_height(const PSetNode *root);
bool pset_validate(const PSetNode *root);
#endif
