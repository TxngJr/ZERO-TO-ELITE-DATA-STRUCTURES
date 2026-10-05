# Chapter 051 — Range Tree

## Goal

Range Tree supports orthogonal range searching over multidimensional points.

This chapter builds a **static 2D Range Tree** over points:

    (x,y,id)

and answers rectangular queries:

    x in [x_low,x_high)
    y in [y_low,y_high)

Supported operations:
- count points in rectangle
- report points in rectangle
- validate primary-tree and associated-list invariants

## Why "Range Tree"?

A 1D balanced BST can isolate an x-range.

For 2D queries we augment every primary-tree node with an **associated structure sorted by y** for all points in that node's subtree.

Then a query decomposes the x-range into canonical subtrees.

For each fully covered x-subtree:
- do two binary searches in its y-sorted array
- count/report only y values in [y_low,y_high)

## Primary Tree

Input points are copied and sorted by:

    x, then y, then id

A balanced static tree is built by repeated median selection.

Each node stores one point plus:
- left child
- right child
- min_x/max_x for its subtree
- y_indices[] containing every subtree point sorted by y

Because construction always chooses medians, height is:

    O(log n)

## Associated y Array

For a node N:

    N.y_indices

contains exactly the point indices in N's subtree ordered by:

    y, then x, then id, then stable internal index

Children already have sorted y arrays.

Build merges:
- left associated array
- node's own point
- right associated array

in linear time per subtree.

Across all levels:

    Theta(n log n)

## Rectangle Query

For node subtree x-span:

    [min_x, max_x]  (coordinate bounds, not API interval)

Three cases:

### No x overlap

If:

    max_x < x_low
or
    min_x >= x_high

skip subtree.

### Full x coverage

If:

    x_low <= min_x
and
    max_x < x_high

the entire subtree satisfies x.

Use binary search in y_indices:
- first y >= y_low
- first y >= y_high

Difference gives count in:

    Theta(log n)

For reporting, emit the matching y slice.

### Partial x coverage

Check the node point itself and recurse into children.

## Canonical Subtrees

A balanced 1D range search decomposes an interval into O(log n) canonical pieces around two boundary paths.

Each fully covered canonical subtree uses y binary search.

Therefore standard 2D Range Tree counting:

    O(log^2 n)

Reporting:

    O(log^2 n + k)

where k is number of reported points.

## Static Design

This chapter intentionally uses a static tree.

Why?
- associated y arrays are compact and simple
- build invariant is easy to inspect
- query complexity is clean

Dynamic 2D range trees require update logic across many associated structures and are substantially more complex.

## Duplicate Coordinates

Multiple points may share:
- x
- y
- both coordinates

Each point record also carries:

    id

The structure treats each input record as an independent point.

## Memory

At each primary-tree level, every point appears in one associated y array.

There are O(log n) levels.

Therefore:

    Theta(n log n)

associated storage.

This is the classic time-space trade-off:
extra memory makes 2D queries faster.

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n log n) |
| rectangle count | O(log^2 n) |
| rectangle report | O(log^2 n + k) |
| storage | Theta(n log n) |
| update | unsupported in this static implementation |

## Range Tree vs KD-Tree

Range Tree:
- strong worst-case orthogonal range guarantees
- higher O(n log n) storage in 2D

KD-Tree:
- usually O(n) storage
- excellent spatial partitioning
- different query behavior and bounds

Chapter 054 studies KD-Trees directly.

## Range Tree vs R-Tree

Range Tree is an in-memory comparison/search structure for orthogonal points.

R-Tree (Chapter 057):
- groups spatial bounding rectangles
- designed for spatial objects and storage/index workloads
- has different balancing/search behavior

## Files

- src/int_range_tree.*
- tests/test_range_tree.c
- examples/range_tree_demo.c
- benchmarks/range_query_benchmark.c
- standard learning artifacts
