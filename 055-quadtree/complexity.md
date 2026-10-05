# Complexity — Quadtree

There is no universal logarithmic guarantee.

Typical well-distributed insert:
    O(depth)

Range query:
    proportional to visited nodes + output

Worst case:
    O(n)

Storage:
    O(n + allocated nodes)

Tree depth is capped by max_depth in this implementation.
