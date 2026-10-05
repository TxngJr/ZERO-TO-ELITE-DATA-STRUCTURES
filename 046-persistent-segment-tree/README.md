# Chapter 046 — Persistent Segment Tree

## Goal

Persistent data structure preserves old versions after updates.

Instead of modifying one Segment Tree in place:

    version 0 --update--> version 1 --update--> version 2

we keep every root:

    root[0]
    root[1]
    root[2]

A point update creates a new root and new nodes only along the modified root-to-leaf path.

Unchanged subtrees are shared.

This chapter implements full persistence for:
- point set from any historical version
- range sum
- range minimum
- point get
- branching versions
- node-count inspection
- structural validation

All ranges are half-open:

    [left,right)

## Ephemeral vs Persistent

Ephemeral structure:
- latest update overwrites prior state

Persistent structure:
- old versions remain queryable

Full persistence:
- a new version may be created from **any existing version**, not only the latest one.

Example:

    v0
    ├── v1
    │   └── v3
    └── v2

v2 and v3 can represent independent branches.

## Path Copying

Suppose array length n=8 and we update index 5.

Only nodes covering index 5 are copied:

    root
      -> one child
          -> one child
              -> leaf

Sibling subtrees are reused by index/pointer.

Cost:

    O(log n) new nodes

instead of copying the whole tree:

    Theta(n)

## Structural Sharing

If a node interval does not contain the updated index:

    new_version.child = old_version.child

The old node is immutable.

This guarantees:
- old versions cannot be changed accidentally
- many versions reuse most memory

## Arena / Node Pool

Teaching implementation stores immutable nodes in a growable array and references children by node index.

Node index 0 is reserved as invalid/null.

Benefits:
- realloc can move the pool without invalidating child references
- ownership is simple: free one arena at the end
- rollback after allocation failure is easy by restoring node_count

Each version stores only one root index.

## Point Set

To create new version from base version:

    persistent_segment_tree_point_set(
        tree,
        base_version,
        index,
        value,
        &new_version
    )

Recursive algorithm:

1. clone current node
2. descend only into the child containing index
3. at leaf store new value
4. on return recompute sum/min in copied ancestors
5. append new root to versions[]

The old root and untouched subtrees remain unchanged.

## Transactional Failure

During update, several nodes may be appended to the arena.

Before mutation:

    old_node_count = node_count

If any allocation fails:
- restore node_count
- do not append a version root

Because newly created nodes are unreachable until the root is published, rollback is safe.

## Version Immutability

After a version root is stored:
- its reachable nodes are never modified
- descendants are immutable
- future updates only append new nodes

This is the central invariant.

## Query

Range query is the same recursion as a static Segment Tree, except it starts from a chosen historical root.

For every version:

    range_sum(version,l,r)
    range_min(version,l,r)

cost:

    O(log n)

## Memory Complexity

Initial version:

    Theta(n) nodes

Each point-set version:

    O(log n) new nodes

After m updates:

    O(n + m log n)

Version-root array:

    O(m)

This is far smaller than copying an n-element array for every version:

    Theta(mn)

## Persistent Segment Tree Applications

- historical snapshots
- time-travel queries
- offline order statistics
- versioned configuration/state
- functional programming
- competitive programming queries over prefixes
- persistent indexes

Later Chapter 092 revisits persistent data structures more broadly.

## Sum/Min Contract

Nodes store:
- int64_t sum
- int64_t min

Caller must keep aggregate arithmetic representable in int64_t.

## Complexity

| Operation | Complexity |
|---|---:|
| initial build | Theta(n) |
| point set -> new version | O(log n) time + O(log n) nodes |
| point get | O(log n) |
| range sum/min | O(log n) |
| version root lookup | O(1) |
| storage after m updates | O(n + m log n) |

## Files

- src/persistent_segment_tree.*
- tests/test_persistent_segment_tree.c
- examples/persistent_demo.c
- benchmarks/version_memory_benchmark.c
- standard learning artifacts
