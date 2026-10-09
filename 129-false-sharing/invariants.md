# Invariants

1. cache-line size is a nonzero power of two.
2. base address is cache-line aligned.
3. stride >= sizeof(_Atomic uint64_t).
4. stride respects atomic alignment.
5. count × stride does not overflow.
6. every counter pointer is correctly aligned for atomic uint64_t.
7. logical counters never overlap.
8. relative line number = (counter_ptr - base) / line_size.
9. packed and padded layouts differ only in stride/layout, not atomic correctness.
10. parallel increment joins all successfully created threads before completion.
