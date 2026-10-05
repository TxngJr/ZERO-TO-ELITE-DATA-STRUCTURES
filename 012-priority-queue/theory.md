# Theory — Priority Queue

## ADT first

Priority Queue says which item is eligible to leave next. It does not require a heap.

Possible representations:
- unsorted list
- sorted list
- binary heap
- d-ary heap
- balanced tree
- specialized heaps

Choosing representation depends on operation mix.

## Heap is partially ordered

Max-heap guarantees ancestors dominate descendants along parent edges, but does not globally sort siblings or subtrees.

Therefore:
- root maximum is immediate
- second-largest is one of root's children, not necessarily index 1 under every generalized representation
- scanning heap array does not produce sorted order

## Complete-tree indexing

Array packing removes per-node pointers and usually gives better locality than pointer-based complete tree.

This is a strong example of choosing representation from shape invariant.

## Comparator design

Real priority queues often accept a comparator.

This chapter hard-codes larger integer priority = higher priority to make heap mechanics visible.

Generic/comparator versions will make more sense after language-specific/generic-container chapters.

## Starvation is not a data-structure bug

If policy always chooses highest priority, low-priority items may wait indefinitely.

Solutions such as aging change effective priority over time and belong to scheduling policy, even if implemented using priority structures.
