# Complexity — Octree

No universal logarithmic guarantee.

Insert:
    O(depth) when subdivision is useful

Box query:
    O(visited nodes + output)

Worst case:
    O(n)

Storage:
    O(points + nodes)

Branching factor is 8, so empty-child allocation policy matters more than in binary trees.
