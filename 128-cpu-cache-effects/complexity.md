# Complexity

Let A = associativity and N = number of accesses.

| Operation | Time | Space |
|---|---:|---:|
| one access | O(A) |
| N accesses | O(NA) |
| reset | Θ(sets × A) |
| validate | Θ(sets × A) |

For small fixed associativity, per-access work is effectively constant.
