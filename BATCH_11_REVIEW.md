# Batch 11 — Negative Review and Audit

## Scope

- 031 Ternary Search Tree
- 032 B-Tree
- 033 B+ Tree

## TST Review

Checked:
- low/high moves do not consume an input byte
- equal move consumes exactly one matched byte
- empty key is tree-level state, not fake sentinel symbol
- terminal and prefix semantics remain distinct
- pruning removes only nodes with no terminal/low/equal/high state
- lexicographic traversal order is low → symbol/terminal → equal → high
- byte contract remains distinct from Unicode code-point/collation semantics
- lookup performance is not falsely presented as self-balanced

## B-Tree Review

Checked:
- allocation overflow guard uses SIZE_MAX/2 without wraparound
- minimum degree t>=2
- max keys 2t-1 and non-root min t-1
- split moves median and corresponding children correctly
- insertion never intentionally descends into a full child
- delete handles leaf/internal predecessor/internal successor cases
- underfull descent repaired by borrow or merge
- root shrinks after final separator removal
- validator checks global key ranges, occupancy and equal leaf depth

## B+ Tree Review

Checked:
- logical size counts leaf keys only
- internal separators are copies: key[i] = min(child[i+1])
- equality routes to right child
- leaf split repairs next pointer
- leaf and internal borrow/merge are representation-specific
- internal rebalancing moves child pointers then recomputes separators
- deleting first leaf key refreshes ancestor separators
- root shrinks when internal root has one child
- leaf chain is checked against exact left-to-right leaf order
- range scan descends once then follows leaf links

## Testing Review

- TST randomized set: 20,000 operations
- B-Tree randomized differential: 30,000 ops for each t=2,3,8
- B+ randomized set/range: 25,000 ops for each t=2,3,8
- validators run after mutation-heavy sequences
- B+ range output checked against a boolean reference universe

## Complexity Review

TST:
- symbol-level shape sensitive; not claimed self-balancing

B-Tree:
- O(log_t n) height and page-oriented motivation

B+ Tree:
- O(log n + k) range with linked leaves for fixed fanout

## Definition of Done

Batch 11 is complete only when Fedora CI configures, builds Chapters 001–033 with warnings + ASan/UBSan and passes the complete CTest suite.
