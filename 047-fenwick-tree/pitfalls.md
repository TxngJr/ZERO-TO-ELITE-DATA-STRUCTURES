# Pitfalls — Fenwick Tree

- mixing external 0-based with internal 1-based indices
- calling lowbit(0) in an update loop
- using i < n instead of i <= n internally
- off-by-one prefix definitions
- assuming range min = prefix_min(r)-prefix_min(l)
- forgetting subtraction/inverse requirement for arbitrary range sums
- building with O(n log n) updates when linear build is available
- integer overflow in sums or set delta
