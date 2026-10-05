# Chapter 054 — KD-Tree

## Goal

KD-Tree partitions multidimensional points by alternating coordinate axes.

This chapter implements a **static balanced 2D KD-Tree** for:

- rectangle count
- rectangle report
- nearest-neighbor search
- structural validation

Point:

    (x, y, id)

Rectangle query uses half-open bounds:

    x in [x_low, x_high)
    y in [y_low, y_high)

## Split Rule

At depth d:

    axis = d % 2

So:
- depth 0 splits by x
- depth 1 splits by y
- depth 2 splits by x
- ...

For each subtree:
1. sort by current axis
2. choose median
3. recurse on left/right halves

Because the median is chosen by element count, height is O(log n).

## Important Build-Cost Honesty

This teaching implementation calls comparison sort independently for every recursive subproblem.

Therefore build cost is:

    O(n log^2 n)

not the optimal O(n log n).

Why keep it?

- code is transparent
- median partition invariant is easy to inspect
- focus remains on KD search/pruning

Production implementations can use:
- linear-time/expected-linear selection
- presorted arrays
- in-place nth-element style partitioning

to reduce build cost.

## Bounding Boxes

Every node caches the axis-aligned bounding box of its entire subtree:

    min_x, max_x
    min_y, max_y

and:

    subtree_size

These caches support aggressive pruning.

## Rectangle Query

For query rectangle Q and subtree box B:

### No intersection

Skip entire subtree.

### Box fully inside query

For count:

    return subtree_size

in O(1).

For report:

    emit all points in subtree.

### Partial intersection

Test node point and recurse into children.

For a balanced 2D KD-Tree, classical orthogonal range searching has:

    O(sqrt(n) + k)

query/report behavior in 2D, where k is output size, under the standard static KD-tree analysis.

Counting can exploit fully covered subtree_size summaries.

## Nearest Neighbor

For query point q:

1. measure distance to current node point
2. compute lower-bound distance from q to each child bounding box
3. visit more promising child first
4. prune child if its bounding-box lower bound is greater than current best

This is branch-and-bound.

## Bounding-Box Distance Lower Bound

If q is inside a box on one axis:

    axis contribution = 0

If q lies outside:

    contribution = distance to nearest box face

Squared lower bound:

    dx^2 + dy^2

Any point inside that subtree must be at least this far away.

## Integer Overflow Safety

Coordinates are int64_t.

Distance subtraction is performed after converting coordinates to long double:

    (long double)x1 - (long double)x2

This avoids overflowing int64_t during coordinate subtraction or squaring.

Nearest-neighbor API returns squared distance as:

    long double

## Tie Rule

If two points have equal squared distance, choose lexicographically by:

    x, then y, then id

This makes tests deterministic.

## KD-Tree Is Not a BST on One Global Key

Ordering changes with depth.

At an x-split:
- left subtree x <= node.x
- right subtree x >= node.x

At a y-split:
- left subtree y <= node.y
- right subtree y >= node.y

Do not apply ordinary one-key BST reasoning globally.

## KD-Tree vs Range Tree

Range Tree (Chapter 051):
- 2D rectangle count O(log^2 n)
- report O(log^2 n + k)
- Theta(n log n) storage

KD-Tree:
- Theta(n) storage
- classical 2D orthogonal range O(sqrt(n)+k)
- natural nearest-neighbor pruning
- lower memory overhead

## KD-Tree vs Spatial Hashing / R-Tree

Later:
- Chapter 058 Spatial Hashing: grid/hash-based locality
- Chapter 057 R-Tree: bounding-rectangle hierarchy, especially storage/spatial objects

## Complexity

For this implementation:

| Operation | Complexity |
|---|---:|
| build | O(n log^2 n) |
| height | O(log n) from median construction |
| rectangle query | classical O(sqrt(n)+k) in 2D |
| nearest neighbor | often strongly pruned, worst-case O(n) |
| storage | Theta(n) |

Nearest-neighbor complexity is data/query dependent; never claim guaranteed O(log n).

## Files

- src/int_kd_tree.*
- tests/test_kd_tree.c
- examples/kd_tree_demo.c
- benchmarks/kd_query_benchmark.c
- standard learning artifacts
