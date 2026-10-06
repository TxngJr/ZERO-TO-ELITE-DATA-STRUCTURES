# Complexity

ให้ `n` = input count, `u` = unique count, `C` = table capacity.

| Operation | Expected | Worst | Space |
|---|---:|---:|---:|
| build | O(n) | O(nC) pathological probing | Θ(C) |
| contains | O(1) | O(C) | O(1) |
| validate | O(C) expected, because contains probes | O(C²) | O(1) |
| free | Θ(1) allocations | Θ(1) allocations | — |

Capacity policy ตั้งเป้า load factor <= 0.5 เพื่อให้ expected probe length ต่ำ แต่ไม่ได้เปลี่ยน theoretical worst-case ของ linear probing.

Storage bytes ใน implementation นับ:
`sizeof(object) + C*sizeof(int) + C*sizeof(uint8_t)`.
