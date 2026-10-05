# Chapter 050 — Interval Heap

## Goal

Interval Heap implements a Double-Ended Priority Queue (DEPQ):

- insert
- peek minimum
- peek maximum
- pop minimum
- pop maximum

Each complete-binary-tree node stores an interval:

    [low, high]

with:

    low <= high

The final node may hold one element; this implementation stores that singleton as:

    low == high

## Core Invariant

For every child node C of parent P:

    P.low <= C.low <= C.high <= P.high

Therefore:
- all low endpoints form a min-heap
- all high endpoints form a max-heap

So the root contains:

    root.low  = global minimum
    root.high = global maximum

## Complete Tree Layout

Nodes are stored in an array.

For 0-based node index i:

    parent = (i-1)/2
    left   = 2i+1
    right  = 2i+2

Each node normally stores two values.

For element count n:

    node_count = ceil(n/2)

The tree remains complete because insertions/removals only change the final node.

## Insertion — Even Current Size

If the heap currently has an even number of elements, all nodes are full.

A new singleton child node is created.

Compare x with parent interval:

1. x < parent.low
   - move parent.low down to the new singleton
   - sift x upward through the min-side endpoints

2. x > parent.high
   - move parent.high down
   - sift x upward through the max-side endpoints

3. parent.low <= x <= parent.high
   - x can remain in the new singleton

## Insertion — Odd Current Size

The final node is a singleton y.

The new value x completes that node:

    low=min(x,y)
    high=max(x,y)

Only one side can newly violate the parent interval because y was already inside the parent interval.

Then:
- new low may sift up the min side
- or new high may sift up the max side

## Pop Minimum

The minimum is root.low.

A replacement value is taken from the final node, then sifted downward through low endpoints.

At each step:
- choose the child with the smaller low
- move that child low upward
- if replacement would exceed that child's high, exchange with the high endpoint
- continue downward

The interval-order repair is what distinguishes interval-heap deletion from ordinary min-heap deletion.

## Pop Maximum

Symmetric:
- remove root.high
- choose child with larger high
- repair against child.low when necessary
- sift downward through the max side

## Singleton Last Node

When total element count is odd:

    last.low == last.high

The singleton conceptually represents interval:

    [x,x]

and must still be contained in its parent interval.

## Duplicates

Duplicates are allowed.

Example:

    5, 5, 5

is valid.

This matters because a priority queue is a multiset-like container rather than a unique-key map.

## Complexity

Let n be number of elements.

| Operation | Complexity |
|---|---:|
| size / empty | O(1) |
| peek min | O(1) |
| peek max | O(1) |
| insert | O(log n) |
| pop min | O(log n) |
| pop max | O(log n) |
| storage | Theta(n) |

This implementation grows its node array geometrically, so insertion is amortized O(log n) including occasional allocation growth.

## Interval Heap vs Two Separate Heaps

A DEPQ can also be built from coordinated min/max heaps.

Interval Heap embeds both orderings in one complete tree:
- low endpoints carry min ordering
- high endpoints carry max ordering
- the interval-containment invariant links them

## Interval Heap vs Min-Max Heap

Both support efficient min and max access.

Interval Heap:
- two elements per node
- min/max orderings live on different endpoints

Min-Max Heap:
- one element per node
- alternating min/max levels

## Validation

Validator checks:
1. node count matches ceil(size/2)
2. low <= high in every full node
3. singleton last node has low == high
4. every child interval is contained in its parent interval
5. root endpoints therefore represent global bounds

## Files

- src/int_interval_heap.*
- tests/test_interval_heap.c
- examples/interval_heap_demo.c
- benchmarks/depq_benchmark.c
- standard learning artifacts
