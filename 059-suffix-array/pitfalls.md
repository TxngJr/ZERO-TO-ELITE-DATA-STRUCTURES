# Pitfalls — Suffix Array

- storing actual suffix strings and wasting Theta(n²) memory
- claiming qsort doubling is O(n log n)
- forgetting second-rank end sentinel ordering
- overflowing doubling span
- mixing suffix index with suffix rank
- defining LCP index inconsistently
- using strcmp on binary text containing byte 0
- returning occurrence positions without documenting SA order
