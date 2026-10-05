# Theory — Heap

Heap has two invariants:

1. shape: complete binary tree
2. order: parent <= child for min-heap

Array representation encodes shape implicitly.

For n>0 complete binary tree edge-height is floor(log2 n).

BUILD-HEAP aggregate analysis:
nodes of height h are at most about n/2^(h+1), and each costs O(h).

So total work is:

    sum O(n*h/2^(h+1)) = O(n)

Together with Omega(n), BUILD-HEAP is Theta(n).

Heap order is a partial order, not a global sorted order.
