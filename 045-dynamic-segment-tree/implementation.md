# Implementation — DynamicSegmentTree

Tree:
- domain_left
- domain_right
- root pointer
- node_count

Node:
- int64_t sum
- left pointer
- right pointer

Intervals are implicit in recursion arguments.

Point-add allocation rolls back newly created empty nodes on failure.

Zero-only empty nodes are pruned.
