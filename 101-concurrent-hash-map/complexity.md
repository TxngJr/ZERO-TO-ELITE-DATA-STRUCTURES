# Complexity

Let n = entries, b = buckets, α=n/b.

| Operation | Expected | Worst |
|---|---:|---:|
| get | O(1+α) | O(n) |
| put existing | O(1+α) | O(n) |
| put new excluding resize | O(1+α) | O(n) |
| resize | Θ(n+b) | Θ(n+b) |
| remove | O(1+α) | O(n) |
| validate | Θ(n + local duplicate checks) | O(n²) pathological bucket |

Amortized insertion includes occasional Θ(n) resize. Concurrent wall time additionally includes RW-lock/bucket contention.
