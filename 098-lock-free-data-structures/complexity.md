# Complexity

Let C = contention/retry count.

| Operation | Local work | Progress |
|---|---:|---|
| push | O(1 + C) | lock-free under lock-free atomic assumption |
| pop | O(1 + C) | lock-free under same assumption |
| approximate size | O(1) | diagnostic |
| quiescent validate | O(current size) | offline |

No finite per-thread retry bound is promised; therefore push/pop are not wait-free.
