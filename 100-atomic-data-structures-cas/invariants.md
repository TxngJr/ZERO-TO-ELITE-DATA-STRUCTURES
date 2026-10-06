# Invariants

1. word_count = ceil(nbits/64).
2. padding bits after logical end remain zero.
3. bit operations address exactly one logical bit.
4. tagged state stores value/version atomically in one 64-bit object.
5. successful tagged CAS increments version exactly once.
6. stale expected snapshot is updated with observed state on CAS failure.
7. version/value overflow is rejected by increment path.
8. lock-free progress claims require runtime atomic lock-free support.
