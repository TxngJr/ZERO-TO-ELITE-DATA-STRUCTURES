# Invariants — MaxPriorityQueue

## Representation

1. size <= capacity
2. capacity==0 implies data==NULL
3. capacity>0 implies data!=NULL
4. logical heap occupies data[0..size)

## Heap-order

For every child index c>0:

    data[parent(c)].priority >= data[c].priority

## Push preservation

Appending may violate only edges on the path from new leaf to root.

sift_up swaps upward until the violated edge disappears.

All unrelated subtrees remain valid.

## Pop preservation

Replacing root with last item may violate only edges on a path downward.

sift_down repeatedly swaps with higher-priority child until parent dominates both children.

## Behavioral postcondition

peek/pop on non-empty queue returns an item whose priority equals the maximum priority currently stored.

Tie value/order is unspecified.
