# Complexity

ให้ R×C เป็น logical matrix และ TR×TC เป็น tile.

| Operation | Time |
|---|---:|
| get/set | Θ(1) |
| fill/copy dense | Θ(RC) |
| row-order sum | Θ(RC) |
| tile-order sum | Θ(RC) |
| tiled transpose | Θ(RC) |
| validate | Θ(padded storage) |

Asymptotic complexity เท่ากัน แต่ memory-access locality ต่างกัน ซึ่งเป็นหัวใจของบทนี้.
