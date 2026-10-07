# Complexity

Key width K = 64 nibbles.

| Operation | Time |
|---|---:|
| lookup | O(K) |
| insert/update | O(K) |
| root hash | Θ(number of nodes) |
| validate | Θ(number of nodes) |
| free | Θ(number of nodes) |

Because K is fixed at 64, lookup/insert are bounded independent of stored key count.
