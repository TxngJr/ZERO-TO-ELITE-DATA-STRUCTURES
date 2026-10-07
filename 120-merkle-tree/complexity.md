# Complexity

Let N = leaf count, L = leaf byte size, H = ceil(log2 N).

| Operation | Time | Space |
|---|---:|---:|
| build leaves | Θ(NL) hashing | Θ(N) hashes |
| build internal levels | Θ(N) hash operations | Θ(N) hashes |
| root lookup | Θ(1) | Θ(1) |
| proof generation | Θ(H) | Θ(H) |
| proof verification | Θ(L + H) | Θ(1) working hash |
| validate | Θ(N) hash operations | Θ(1) extra |

All tree levels together use less than about 2N hashes for N>1.
