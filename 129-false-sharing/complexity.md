# Complexity

Let C = counters, T = threads, I = increments/thread.

| Operation | Time | Space |
|---|---:|---:|
| create counter array | Θ(C) initialization | Θ(C × stride) |
| get/load counter | Θ(1) | Θ(1) |
| share-line query | Θ(1) | Θ(1) |
| line occupancy | Θ(C) | Θ(1) |
| reset | Θ(C) | Θ(1) |
| parallel increment | Θ(TI) total atomic operations | Θ(T) thread metadata |
| validate | Θ(C) | Θ(1) |
