# Invariants — IntFenwickTree

For internal index i in 1..n:

    tree[i]
      = sum of logical array over
        [i-lowbit(i), i)

in 0-based half-open indexing.

Consequences:
- prefix traversal selects disjoint blocks
- point update touches exactly containing ancestor blocks
- range sum is prefix(right)-prefix(left)
