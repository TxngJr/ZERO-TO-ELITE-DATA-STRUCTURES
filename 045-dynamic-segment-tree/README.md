# Chapter 045 — Dynamic Segment Tree

## Goal

Static Segment Tree allocates Theta(n) storage for every array position.

But suppose coordinate universe is huge:

    [0, 10^18)

and only a few thousand coordinates are ever touched.

Allocating a static tree for the whole universe is impossible.

Dynamic Segment Tree allocates nodes **only along paths that are actually updated**.

This chapter implements:
- uint64_t coordinate domain [domain_left, domain_right)
- point add
- point get
- range sum
- node count
- structural validation
- pruning of zero-only empty paths

Untouched coordinates logically contain zero.

## Implicit Intervals

A node does not store its interval bounds.

The recursion carries:

    [left,right)

Root:
    [domain_left,domain_right)

Children:
    [left,mid)
    [mid,right)

with overflow-safe unsigned midpoint:

    mid = left + (right-left)/2

Because right > left and unsigned subtraction is defined, this avoids left+right overflow.

## Missing Node Semantics

A NULL child means:

    every value in that entire interval is zero

Therefore query can immediately return zero on NULL.

This is the key source of sparse memory savings.

## Point Add

To execute:

    value[x] += delta

walk one path from root toward leaf x.

Allocate missing nodes on that path.

At leaf:
    sum += delta

On return:
    sum[parent] =
        sum(left child or 0)
      + sum(right child or 0)

Height depends on coordinate universe span U:

    O(log U)

not on number of stored points.

## Pruning

If after update a node has:
- sum == 0
- left == NULL
- right == NULL

it can be freed.

This commonly removes a leaf/path when a point is returned to zero.

Important:
an internal node with sum==0 but nonempty children cannot be removed because positive and negative values might cancel.

## Sparse Sharing

K updated coordinates do not necessarily require K*height distinct nodes because their paths share prefixes.

Worst-case storage:

    O(K log U)

Typical node count may be lower.

## Range Sum

Query [ql,qr):

No overlap:
    0

NULL node:
    0

Full cover:
    cached node sum

Partial:
    recurse existing children and add

Missing subtrees terminate immediately.

## Transactional Allocation Failure

A point update can allocate several nodes.

Teaching implementation ensures:
- sums are modified only after deeper recursion succeeds
- a newly allocated empty node is removed if deeper allocation fails

Therefore allocation failure does not leave a partial logical point update.

## Coordinate vs Array Index

Static Segment Tree:
- dense indices 0..n-1
- storage tied to n

Dynamic Segment Tree:
- potentially enormous coordinate domain
- storage tied to touched paths

The abstract range-query idea is the same; representation changes.

## Complexity

Let:

    U = domain_right - domain_left
    K = number of nonzero/touched coordinates conceptually

Point add:
    O(log U)

Point get:
    O(log U)

Range sum:
    O(log U) canonical interval decomposition, with NULL branches terminating early

Storage:
    O(K log U) worst case

Node validation:
    O(number of allocated nodes)

## Domain Contract

Require:

    domain_left < domain_right

Coordinates are uint64_t.

The interval is half-open:

    [domain_left, domain_right)

This can represent extremely large finite universes, including high values near UINT64_MAX, though an exclusive upper bound of 2^64 itself cannot be represented in uint64_t.

## Integer Sum Contract

Stored sums are int64_t.

Caller must keep all point values and subtree sums representable in int64_t.

## Files

- src/dynamic_segment_tree.*
- tests/test_dynamic_segment_tree.c
- examples/dynamic_demo.c
- benchmarks/sparse_universe_benchmark.c
- standard learning artifacts
