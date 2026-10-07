# Complexity

Let N = rows and K = query output count.

| Operation | Time |
|---|---:|
| build primary sort | O(N log N) |
| build secondary sort | O(N log N) |
| primary lookup | O(log N) |
| secondary equality | O(log N + K) |
| secondary range | O(log N + K) |
| validate | O(N) |
| space | Θ(N) |

This chapter analyzes logical index work; physical page I/O depends on the storage representation.
