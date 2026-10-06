# Complexity

ให้ F = จำนวน free extents, A = live allocations.

| Operation | Time |
|---|---:|
| first-fit allocate | O(F) |
| release lookup | O(A) |
| sorted free insertion | O(F) |
| coalesce neighbors | O(1) หลัง insertion point |
| free-byte query | Θ(1) จาก capacity-allocated |
| largest free block | Θ(F) |
| full validate | O(A² + A·F + F) |

Space metadata = O(A+F).
