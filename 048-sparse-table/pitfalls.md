# Pitfalls — Sparse Table

- trying to support frequent updates
- using overlapping-block trick for sum
- using ceil(log2) instead of floor(log2)
- right block starting at right-block_length incorrectly
- building entries whose interval exceeds n
- mixing inclusive ranges with half-open API
- assuming every associative operation is idempotent
- allocating levels*n without overflow checks
