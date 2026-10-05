# Complexity — Spatial Hashing

Expected insert:
    O(1)

Range query:
    expected O(touched cells + candidate points)

Storage:
    Theta(points + occupied cells + buckets)

Worst-case behavior is not guaranteed logarithmic.
Cell-size choice can dominate real performance.
