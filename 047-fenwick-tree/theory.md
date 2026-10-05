# Theory — Fenwick Tree

## lowbit

For positive integer i:

    lowbit(i) = i & -i

In unsigned arithmetic this isolates the least-significant set bit.

Fenwick traversal uses the binary decomposition of prefix length.

## Query proof

Each query step subtracts lowbit(i), selecting a disjoint stored block ending at the current prefix boundary.

Those blocks partition [0,end), so their sums add to the prefix sum.

## Update proof

Each update step adds lowbit(i), moving to the next stored block that also contains the updated point.

Therefore every affected cached block receives delta exactly once.
