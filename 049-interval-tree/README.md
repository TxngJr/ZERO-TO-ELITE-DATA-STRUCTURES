# Chapter 049 — Interval Tree

## Goal

Interval Tree answers overlap queries over intervals efficiently.

This chapter uses half-open intervals:

    [low, high)

with invariant:

    low < high

The concrete structure is an AVL tree keyed lexicographically by:

    (low, high)

and every node is augmented with:

    max_high

the maximum interval endpoint in its entire subtree.

Supported operations:
- insert unique interval
- remove exact interval
- contains exact interval
- find any overlapping interval
- count all overlapping intervals
- validate AVL/BST/max_high invariants

## Overlap Predicate

Two half-open intervals:

    A=[a,b)
    B=[c,d)

overlap iff:

    a < d && c < b

Touching endpoints do not overlap:

    [1,4) and [4,7)

## Why max_high?

Suppose we are searching for query:

    Q=[q_low,q_high)

If a left subtree has:

    left.max_high <= q_low

then every interval in that subtree ends at or before the query starts.

Therefore none can overlap Q.

That entire subtree can be pruned.

## Search Any Overlap

At node N:

1. if N overlaps query -> return it
2. if left exists and left.max_high > q_low:
       search left
3. otherwise:
       search right

Because the tree is ordered by low endpoint and max_high summarizes right endpoints, this pruning avoids scanning every interval.

## Counting All Overlaps

For reporting/counting:
- prune any subtree whose max_high <= query.low
- once node.low >= query.high, the node and all nodes in its right subtree start too late
- otherwise test current node and recurse as needed

Output-sensitive reporting can be built on the same pruning rules.

## AVL Balance

A plain interval BST can become height Theta(n).

This implementation uses AVL rotations so tree height stays O(log n).

Each node caches:
- height
- max_high

After insertion/deletion/rotation both caches are recomputed.

## Node Invariant

For node N:

    max_high(N)
      = max(
          N.high,
          max_high(N.left),
          max_high(N.right)
        )

and:

    height(N)
      = 1 + max(height(left), height(right))

AVL balance factor:

    -1 <= height(left)-height(right) <= 1

## Complexity

With AVL balancing:

| Operation | Complexity |
|---|---:|
| exact insert | O(log n) |
| exact remove | O(log n) |
| exact contains | O(log n) |
| find any overlap | O(log n) typical/worst under standard interval-tree pruning with balanced ordering |
| count/report all overlaps | O(log n + k) when pruning is effective, with k overlaps; degenerate query distributions can visit more nodes |
| storage | Theta(n) |

The important point is that augmentation enables subtree pruning; complexity of reporting depends on output and interval distribution.

## Duplicates

Exact duplicate intervals are rejected.

Intervals may share:
- the same low endpoint
- the same high endpoint

Lexicographic (low,high) ordering keeps exact-key behavior deterministic.

## Files

- src/int_interval_tree.*
- tests/test_interval_tree.c
- examples/interval_tree_demo.c
- benchmarks/overlap_benchmark.c
- standard learning artifacts
