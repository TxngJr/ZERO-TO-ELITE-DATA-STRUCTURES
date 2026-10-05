# Pitfalls — Arrays

1. off-by-one index: using i <= n for n-element array
2. confusing array object with pointer
3. using sizeof(pointer) as array length
4. integer overflow in capacity * sizeof(element)
5. assigning realloc result directly and losing old allocation on failure
6. using memcpy on overlapping insert/erase ranges
7. incrementing size before fallible reserve succeeds
8. storing backing pointer then using it after a grow
9. growing capacity by +1 and accidentally producing quadratic append sequence
10. shrinking on every pop and causing resize thrashing
11. assuming Θ(1) random access means equal physical latency for all addresses
12. using row/column loop order without considering layout
13. forgetting that C built-in arrays carry no runtime bounds checks
14. assuming zero capacity and non-NULL pointer contracts are universal across all libraries

When debugging:
- assert size <= capacity
- run ASan/UBSan
- test empty/one-element/boundary indices
- randomize operation sequences
- compare against trusted reference model
