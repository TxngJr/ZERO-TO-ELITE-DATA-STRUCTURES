# Complexity

Let N = vectors, D = dimensions, k = requested neighbors.

| Operation | Time |
|---|---:|
| add vector | Θ(D) amortized |
| L2/cosine distance | Θ(D) |
| exact top-k query | O(N(D + log k)) |
| final result sort | O(k log k) |
| validate | Θ(ND) |

Space = Θ(ND + N + k query workspace).
