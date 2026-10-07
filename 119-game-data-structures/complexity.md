# Complexity

Let E = capacity, P = positioned entities, C = cells, M = candidates in queried cells.

| Operation | Time |
|---|---:|
| spawn | Θ(1) |
| destroy | Θ(1) |
| set/get/remove position | Θ(1) |
| world validate | Θ(E + P) |
| grid rebuild | Θ(C + P) |
| AABB query | O(overlapping cells + M) |
| grid validate | Θ(C + P) |

Space = Θ(E + P + C).
