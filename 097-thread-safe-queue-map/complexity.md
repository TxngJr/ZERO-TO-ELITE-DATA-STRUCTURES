# Complexity

| Operation | Sequential work | Blocking/Contention |
|---|---:|---|
| queue try push/pop | O(1) | mutex acquisition |
| queue wait push/pop | O(1) per wake | may block |
| map get/put/remove | O(chain) | one bucket mutex |
| map size | O(1) | size mutex |
| map validate | O(n + duplicate checks per bucket) | quiescent only |

Fixed bucket count means chain lengths can grow with n; this chapter prioritizes synchronization design over resizing.
