# Complexity

| Operation | Steps | Progress |
|---|---:|---|
| try_push | Θ(1) fixed | wait-free under stated assumptions |
| try_pop | Θ(1) fixed | wait-free under stated assumptions |
| usable_capacity | Θ(1) | local |
| quiescent validate | Θ(1) | offline |

Caller retry loops under full/empty are not included in per-call bound.
