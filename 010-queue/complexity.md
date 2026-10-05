# Complexity — Queue

Let n=current size.

## Circular Queue

peek/front:
    Θ(1)

dequeue:
    Θ(1)

enqueue:
    Θ(1) if capacity available
    Θ(n) on growth
    Θ(1) amortized with geometric growth

space:
    Θ(capacity)

## Linked Queue

peek:
    Θ(1)

enqueue:
    Θ(1) pointer changes plus allocation

dequeue:
    Θ(1)

space:
    Θ(n) nodes

## Naive shifting array queue

dequeue:
    Θ(n)

For n repeated dequeues:
    Θ(n²)

Circular representation avoids the shift.

## BFS preview

For graph adjacency-list BFS:
queue operations are O(1) amortized each; full algorithm is typically Θ(V+E) when each vertex/edge is processed bounded times.
