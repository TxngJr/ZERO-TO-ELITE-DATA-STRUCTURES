# Complexity

Let W = ceil(n/64), R = CAS retries.

| Operation | Best | Worst |
|---|---:|---:|
| bit test | Θ(1) | Θ(1) |
| fetch-or/and/xor update | Θ(1) | Θ(1) primitive cost |
| compare_exchange attempt | Θ(1) | Θ(1) |
| CAS increment | Θ(1) | O(R) retries |
| count/validate | Θ(W) | Θ(W) |

Wall-clock latency under contention depends on cache-line bouncing, scheduler and memory hierarchy.
