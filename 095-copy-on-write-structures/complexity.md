# Complexity

ให้ n=size.

| Operation | Best | Worst |
|---|---:|---:|
| clone | Θ(1) | Θ(1) |
| get | Θ(1) | Θ(1) |
| set | Θ(1) unique | Θ(n) shared |
| push | amortized Θ(1) unique | Θ(n) detach/grow |
| pop | Θ(1) unique | Θ(n) shared |

Space = one shared payload + Θ(1) per handle; every detached branch gets its own payload.
