# Complexity

ให้ `h` = height ของ version ที่ update และ `n` = size.

| Operation | Best | Average* | Worst | New nodes |
|---|---:|---:|---:|---:|
| contains | Θ(1) | O(log n) | Θ(n) | 0 |
| insert new | Θ(1) | O(log n) | Θ(n) | O(h) |
| insert duplicate | Θ(1) | O(log n) | Θ(n) | 0 |
| erase present | Θ(1) | O(log n) | Θ(n) | O(h) |
| erase missing | Θ(1) | O(log n) | Θ(n) | 0 |
| validate | Θ(n) | Θ(n) | Θ(n) | recursion O(h) |

\* Average assumes non-adversarial tree shape; implementation does not randomize or rebalance.

ถ้าใช้ balanced persistent tree จะยกระดับ worst-case update/query เป็น O(log n) พร้อม O(log n) copied nodes.
