# Complexity

Let C = heap capacity, L = live objects, E <= 2L.

| Operation | Time |
|---|---:|
| allocate from free stack | Θ(1) |
| get/set edge/value | Θ(1) |
| add/remove root | Θ(1) |
| mark-sweep collect | Θ(C + E) |
| validate | Θ(C) |

Heap memory = Θ(C). Mark stack is preallocated Θ(C).
