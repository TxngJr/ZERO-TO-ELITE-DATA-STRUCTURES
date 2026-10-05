# Theory — Binary Tree Representation

## Representation Invariant vs Ordering Invariant

Structural invariant:
child/parent links form a valid tree.

Ordering invariant:
keys obey application-specific relation.

Chapter 017 has only structural invariants.
BST in Chapter 019 adds ordering.

## Parent-chain Membership

For a valid node:
following parent repeatedly reaches exactly tree->root.

This makes a useful membership test.

Step bound <= size prevents malformed parent cycles from looping forever.

## Subtree Ownership

A subtree is not separately owned in this API.

Removing subtree transfers nothing; it destroys the entire subtree.

Alternative APIs could:
- detach and return subtree ownership
- reference count
- arena-own all nodes
- use smart pointers in C++

Ownership policy changes API semantics.
