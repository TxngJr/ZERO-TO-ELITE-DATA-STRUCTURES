# Complexity

ให้ S_c = จำนวน slabs ใน selected class, S = slabs ทั้งหมด, K = slots/slab.

| Operation | Teaching implementation |
|---|---:|
| class selection | Θ(1), 5 classes |
| allocate | O(S_c) worst |
| release | O(S) worst |
| trim empty | Θ(S) |
| validate | Θ(S·K) |

Validator ใช้ temporary seen bitmap ต่อ slab จึงตรวจ free-index duplicates ได้เชิงเส้นต่อจำนวน slots.

Production slab allocators มักเพิ่ม partial-slab lists, direct page-to-slab metadata, per-thread/per-CPU caches หรือ class-local fast paths เพื่อให้ hot paths เร็วกว่านี้.
