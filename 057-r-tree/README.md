# Chapter 057 — R-Tree

## Goal

R-Tree indexes spatial rectangles with a balanced hierarchy of:

    Minimum Bounding Rectangles (MBRs)

Unlike Quadtree/Octree:
- space is not divided by fixed grid boundaries
- node regions may overlap
- rectangles are grouped by spatial closeness

This chapter implements a **dynamic 2D R-Tree** with:
- rectangle insertion
- least-enlargement subtree choice
- quadratic node split
- automatic root split
- overlap count/report
- structural validation

All object rectangles use half-open bounds:

    [x_low,x_high) × [y_low,y_high)

## Node Capacity

Teaching constants:

    M = 4 maximum entries
    m = 2 minimum entries for non-root nodes

Insertion temporarily considers:

    M+1 = 5

entries during split.

Small M makes split behavior easy to draw and test.

Production R-Trees choose capacities based on page/block size.

## Leaf vs Internal Entries

Leaf entry:

    object rectangle + id

Internal entry:

    child MBR + child pointer

For every internal entry:

    entry.rect == exact MBR(child)

## Choose Subtree

To insert rectangle R into an internal node, choose child whose MBR needs the smallest area enlargement:

    enlargement =
        area(combine(child_mbr,R))
        - area(child_mbr)

Tie-break:
1. smaller current area
2. fewer entries
3. lower index

This tries to reduce future overlap/wasted coverage.

## Quadratic Split

When a node with capacity M receives one extra entry:

    5 entries

Quadratic split:

### Pick seeds

Choose pair with largest dead space:

    waste =
        area(combine(A,B))
        - area(A)
        - area(B)

These two entries start different groups.

### Distribute remaining entries

For each remaining candidate compute:

    enlargement(group A)
    enlargement(group B)

Choose the entry with greatest preference difference.

Assign it to:
1. smaller enlargement
2. smaller group MBR area
3. smaller group size

Minimum-fill rule ensures each resulting node gets at least m entries.

## Balanced Height

R-Tree splits propagate upward.

If root splits:
- allocate a new internal root
- old root and sibling become its two children

All leaves therefore remain at the same depth.

This is a key R-Tree invariant.

## Overlap Query

For query rectangle Q:

Internal node:
- recurse only into entries whose MBR overlaps Q

Leaf:
- report/count object rectangles that overlap Q

Because MBRs may overlap, query may need to descend multiple branches.

This differs from ordinary BST search.

## Rectangle Arithmetic

Coordinates are int64_t.

Area computation widens endpoints to long double before subtraction:

    width  = (long double)xh - (long double)xl
    height = (long double)yh - (long double)yl

This avoids signed integer overflow in width/height calculation.

## Allocation-Failure Strategy

Before descending through a **full internal node**, implementation preallocates a possible sibling.

This prevents a child split from succeeding only to discover that its full parent cannot allocate the propagated split node.

At the root, a possible new root is also preallocated when the root is full.

This is conservative:
- an insertion may fail early under extreme allocation pressure
- but avoids publishing a structurally half-propagated split

## Complexity

R-Trees are spatial indexes; runtime depends heavily on MBR overlap and data distribution.

Height is logarithmic in entry count because:
- non-root nodes have minimum occupancy
- all leaves share a depth

Typical insertion:
    O(height × node-choice work)

With fixed M=4:
    O(height)

Overlap query:
    depends on number of intersecting MBR branches
    worst case O(n + k)

where k is output count.

Storage:
    O(n)

## R-Tree vs Range Tree

Range Tree:
- points
- strong orthogonal query guarantees
- higher asymptotic memory in 2D

R-Tree:
- rectangles/spatial objects
- dynamic page-like hierarchy
- overlapping MBRs
- practical spatial/database indexing

## R-Tree vs Quadtree

Quadtree:
- fixed geometric subdivision
- child regions do not overlap

R-Tree:
- data-driven rectangle grouping
- sibling MBRs may overlap
- balanced by node occupancy/splits

## Files

- src/int_r_tree.*
- tests/test_r_tree.c
- examples/r_tree_demo.c
- benchmarks/r_tree_benchmark.c
- standard learning artifacts
