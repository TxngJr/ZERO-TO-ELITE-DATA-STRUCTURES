# Complexity

Let D = ceil(N/B) data blocks and K = output size.

| Operation | CPU Time | Logical Data-Block I/O |
|---|---:|---:|
| build | Θ(N) | Θ(D) writes |
| contains | O(log D + log B) | <= 1 read |
| range | O(log D + touched·B) | Θ(touched blocks) reads |
| validate | Θ(N) | 0 modeled I/O |

Directory is assumed RAM-resident in this chapter.
