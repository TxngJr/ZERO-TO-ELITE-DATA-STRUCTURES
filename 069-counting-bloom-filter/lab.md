# Lab 069 — Add, Count, Remove Carefully

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch069_counting_bloom_tests

Tasks:
1. Add one key twice.
2. Remove once and query.
3. Remove second time.
4. Add many keys.
5. Remove only known-live keys.
6. Observe removed keys may remain maybe-present.
7. Trigger saturation using one counter.
