# Complexity

Let A = number of accesses and U = unique lines.

| Operation | Time | Space |
|---|---:|---:|
| analyze trace | expected Θ(A) | O(U) |
| sequential generator | Θ(A) | output Θ(A) |
| strided permutation generator | Θ(A) | output Θ(A) |

The hash set uses expected O(1) insertion/lookup under normal load.
