# Complexity — B+ Tree

Height:
    O(log_t n)

Teaching in-node scans are linear in node key count.

Search/insert/delete:
    O(t log_t n)

For fixed page capacity:
    O(log n)

Range:
    O(t log_t n + k)
or external-memory view:
    O(height + output pages)

Linked leaves make sequential scans efficient.
