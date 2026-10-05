# Complexity — Persistent Segment Tree

Initial build:
    Theta(n)

Point-set new version:
    O(log n) time
    O(log n) new nodes

Historical query:
    O(log n)

After m updates:
    O(n + m log n) nodes

If every version copied the whole array/tree instead:
    Theta(mn) memory

Persistence exchanges extra memory for historical access without destructive updates.
