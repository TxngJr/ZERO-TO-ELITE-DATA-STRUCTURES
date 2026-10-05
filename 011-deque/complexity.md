# Complexity — Deque

## Ring Deque

front/back:
    Θ(1)

pop_front/pop_back:
    Θ(1)

push_front/push_back:
    Θ(1) without growth
    Θ(n) on growth
    Θ(1) amortized with doubling

space:
    Θ(capacity)

## Sliding Window Maximum

n elements, window k:

Each index enters deque once.
Each index leaves at most once from front and/or is removed once from back.

Total deque actions O(n).

Time:
    Θ(n)

Output size:
    n-k+1

Auxiliary index deque:
    O(k) logically, though simple implementation may allocate n slots for convenience. An optimized bounded implementation can allocate k indices.

This distinction between logical maximum occupancy and allocated capacity matters.
