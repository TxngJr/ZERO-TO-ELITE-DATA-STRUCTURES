# Complexity

ให้ T = transactions ใน block, B = blocks.

| Operation | Time |
|---|---:|
| append block | Θ(T) Merkle work |
| block lookup | Θ(1) |
| transaction lookup | Θ(1) |
| proof generation | Θ(T) in this teaching implementation because tree is rebuilt |
| proof verification | O(log T) |
| validate chain | Θ(total transactions) |

Stored space = Θ(B + total transactions).
