# Complexity

ให้ `f=front_len`, `r=rear_len`.

| Operation | Best | Worst | Amortized* |
|---|---:|---:|---:|
| enqueue | Θ(1) | Θ(1) | Θ(1) |
| peek/dequeue | Θ(1) | Θ(r) | Θ(1) |
| size | Θ(1) | Θ(1) | Θ(1) |
| validate | Θ(f+r) | Θ(f+r) | N/A |

*สำหรับ linear history; branching จาก version ก่อน normalization อาจทำ reversal ซ้ำ.
