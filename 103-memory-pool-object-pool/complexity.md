# Complexity

ให้ C = capacity.

| Operation | Time | Extra Space |
|---|---:|---:|
| alloc | Θ(1) | Θ(1) |
| release | Θ(1) | Θ(1) |
| owns | Θ(1) | Θ(1) |
| in-use count | Θ(1) | Θ(1) |
| validate | Θ(C) | Θ(C) temporary seen bitmap |

Reserved payload bytes = `stride * capacity`.

Pool construction = Θ(C) เพราะ initialize free-stack indices.
