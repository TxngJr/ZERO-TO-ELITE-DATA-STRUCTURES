# Complexity

Let V = nodes, E = edges.

| Operation | Time |
|---|---:|
| add node | expected O(1) |
| add edge | expected O(1) endpoint lookup + O(1) append |
| build CSR | Θ(V+E) |
| sort label index | O(V log V) |
| node lookup | expected O(1) |
| typed neighbor query | O(degree) |
| label query | O(log V + matches) |
| BFS | O(V+E) worst |
| validate | Θ(V+E) plus node hash lookups |

Space = Θ(V+E).
