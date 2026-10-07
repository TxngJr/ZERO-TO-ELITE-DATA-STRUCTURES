# Complexity

Let N = rows, F = feature width, D = embedding dims, B = batch size.

| Operation | Time |
|---|---:|
| dataset set row | Θ(F) |
| dataset row view | Θ(1) |
| dataset gather | Θ(BF) |
| Fisher-Yates shuffle | Θ(N) |
| next batch metadata | Θ(1) |
| validate permutation | Θ(N) |
| embedding set row | Θ(D) |
| embedding row view | Θ(1) |
| embedding gather | Θ(BD) |

Space:
- dataset Θ(NF + N)
- batch plan Θ(N)
- embeddings Θ(rows × D)
