# Lab 068 — Definitely Not vs Maybe

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch068_bloom_filter_tests

Tasks:
1. Create m=65536, k=5.
2. Insert 5000 integer keys.
3. Verify every inserted key remains maybe-present.
4. Query disjoint keys.
5. Measure false positives.
6. Increase m.
7. Change k and compare.
