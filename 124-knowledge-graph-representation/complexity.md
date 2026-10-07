# Complexity

Let V = terms, T = triples, M = matches after first-key narrowing.

| Operation | Time |
|---|---:|
| intern existing term | expected O(1) |
| intern new term | expected O(1) |
| add unique triple | expected O(1) |
| build 3 indexes | O(T log T) |
| query with one bound position | O(log T + M) |
| fully wildcard query | Θ(T) |
| validate | O(V + T log-free index checks) |

Space = Θ(total term bytes + V + T).
