# Complexity

Let R = CAS retries due contention.

| Operation | Local work |
|---|---:|
| try_push | O(1 + R) |
| try_pop | O(1 + R) |
| capacity | Θ(1) |
| quiescent size | Θ(1) |
| quiescent validate | Θ(1) |

Memory = Θ(capacity). No allocation occurs after creation.
