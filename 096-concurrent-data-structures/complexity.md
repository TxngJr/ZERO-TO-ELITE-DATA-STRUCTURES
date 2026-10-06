# Complexity

| Operation | Sequential cost | Concurrency cost |
|---|---:|---|
| contains | O(log n) | may wait for mutex |
| insert/remove | O(n) worst | serialized + contention |
| size | O(1) | mutex acquisition |
| snapshot | Θ(n) | holds lock during copy |
| validate | Θ(n) | holds lock during scan |

Asymptotic work does not bound queueing delay. Throughput depends on threads, critical-section length, scheduler, allocator and cache-coherence behavior.
