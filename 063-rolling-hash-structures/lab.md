# Lab 063 — Hash, Reject, Verify

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch063_rolling_hash_tests

Tasks:
1. Build prefix hashes for banana.
2. Compare "ana" ranges [1,4) and [3,6).
3. Inspect raw hash pair.
4. Run verified Rabin-Karp for "ana".
5. Test embedded byte zero.
6. Compute exact LCP of two suffixes.
7. Compare all operations with naive byte logic.
