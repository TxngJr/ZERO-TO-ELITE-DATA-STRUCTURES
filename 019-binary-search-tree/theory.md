# Theory — Binary Search Tree

## Global invariant, not only local intuition

It is not enough to check:

    left child < node < right child

A deep node must satisfy bounds inherited from every ancestor.

Validator therefore carries lower/upper bounds recursively.

## Search-tree partition

At node k:
- all left-subtree keys < k
- all right-subtree keys > k

This partition is what allows pruning.

## Height is the real operation parameter

BST algorithms move along one root-to-node path plus bounded local work.

Thus:
    O(h)

Only a balance invariant lets us replace h by O(log n).

## Deletion correctness

Two-child deletion chooses successor because:
- successor is greater than every left-side key
- successor is the smallest key greater than deleted key in its right subtree

After relinking, inherited ranges remain valid.
