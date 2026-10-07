# Complexity

Let N = record count.

| Operation | Time | Extra Space |
|---|---:|---:|
| serialized-size calculation | Θ(1) | Θ(1) |
| serialize | Θ(N) | output Θ(N) |
| validate blob | Θ(N) CRC | Θ(1) |
| deserialize | Θ(N) | Θ(N) records |

Wire bytes = 32 + 20N.
