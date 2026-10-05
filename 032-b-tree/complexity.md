# Complexity — B-Tree

Height:
    O(log_t n)

Teaching implementation scans keys linearly inside a node.

Search:
    O(t log_t n)

Insert:
    O(t log_t n)

Delete:
    O(t log_t n)

For fixed page capacity/fanout this is O(log n).

External-memory cost emphasizes O(height) page accesses.
