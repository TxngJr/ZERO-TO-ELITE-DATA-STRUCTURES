# Theory — Interval Heap

## Embedded min and max heaps

Containment:

    parent.low <= child.low
    child.high <= parent.high

means:
- following low endpoints downward gives min-heap order
- following high endpoints downward gives max-heap order

The root simultaneously exposes both extremes.

## Why a singleton is [x,x]

With one element in the final node, treating it as interval [x,x]:
- preserves containment language
- makes min/max of that node identical
- simplifies validation

## Deletion repair

During min-side sift-down, a replacement might be larger than a child's high endpoint.

Swapping the replacement with that high endpoint restores the local interval ordering while the displaced value continues down the min side.

Max deletion is symmetric.
