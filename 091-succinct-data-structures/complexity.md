# Complexity

ให้ `n` = number of bits, `W=64`, `S=8` words/superblock.

| Operation | Best | Average | Worst | Extra space |
|---|---:|---:|---:|---:|
| build | Θ(n) | Θ(n) | Θ(n) | Θ(n/W + n/(WS)) words metadata/payload |
| access | Θ(1) | Θ(1) | Θ(1) | Θ(1) |
| rank1/rank0 | Θ(1) | Θ(1) | Θ(1) under fixed S | Θ(1) |
| select1/select0 | Ω(1) | O(log(n/(WS))) | O(log n) with fixed local scan | Θ(1) |
| validate | Θ(n/W) | Θ(n/W) | Θ(n/W) | Θ(1) |

`rank` เป็น fixed bounded scan เพราะ `S=8`; ไม่ควรสรุปจาก Big-O อย่างเดียว เพราะ popcount implementation, branch behavior และ cache residency ของ directory มีผลต่อ latency จริง.

Space ใน implementation นี้ **ไม่ใช่ strict `n+o(n)` proof** เนื่องจาก checkpoint interval คงที่.
