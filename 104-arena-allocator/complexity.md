# Complexity

ให้ B = จำนวน blocks.

| Operation | Time |
|---|---:|
| alloc fast path | Θ(1) |
| alloc grow path | Θ(1) + allocator call |
| calloc | Θ(requested bytes) |
| mark | Θ(1) |
| reset to mark | O(blocks after mark) |
| reset | Θ(B) |
| validate | Θ(B) |

Space includes payload, alignment padding, unused block tails และ headers.
