# Theory — Treap

## Unique shape from two total orders

For unique keys and unique total priority order, a Treap shape is determined by:
- BST ordering by key
- heap ordering by priority

The highest-priority pair becomes root.
Recursively the same applies to left/right key partitions.

## Random BST connection

If priority ranks are a random permutation independent of key order, root is equally likely to be any key.

Left/right subproblems inherit the same random-order property.

This yields expected logarithmic path lengths.

## Probabilistic vs deterministic statement

Invariant correctness is deterministic.

Complexity statement is probabilistic:
- expected O(log n)
- worst O(n)

Never convert expected bound into worst-case guarantee.

## Merge correctness

Precondition:
every key in left < every key in right.

Selecting heap-better root and recursively merging only the boundary child preserves both:
- BST order
- heap order.
