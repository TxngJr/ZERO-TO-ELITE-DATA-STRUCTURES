# Complexity

Let N = rows×cols and P = padded_side².

| Operation | Time |
|---|---:|
| get/set | O(log padded_side) bit interleave |
| row-order sum | O(N log padded_side) |
| physical Z-order sum | Θ(P) |
| dense copy | O(N log padded_side) |
| transpose | O(N log padded_side) |
| validate padding | Θ(P) |

Space = Θ(P), which may exceed Θ(N) because of power-of-two square padding.
