# Implementation — IntBStarTree

Representation:

Node:
- leaf
- count
- keys array capacity max_keys+1
- children array capacity max_children+1

Tree:
- order m
- size
- root

Overflow is temporarily allowed:
- node keys can reach m
- node children can reach m+1

Parent repair then:
1. redistributes with a non-full sibling, or
2. performs 2-to-3 split.

Root overflow creates a new root and two children.

Deletion uses transactional rebuild to preserve invariants and clear failure semantics.
