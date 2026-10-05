# Complexity — R-Tree

Height:
    O(log n) under minimum occupancy and balanced leaves

Insertion with fixed M:
    O(height)

Overlap query:
    distribution dependent
    worst O(n+k)

Storage:
    O(n)

Unlike one-dimensional balanced search trees, spatial overlap can force several branches to be searched.
