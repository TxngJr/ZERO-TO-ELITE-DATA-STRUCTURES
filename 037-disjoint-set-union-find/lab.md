# Lab 037 — Flatten the Forest

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch037_dsu_tests

Tasks:
1. Create 10 singleton components.
2. Union pairs into groups.
3. Predict component count after each successful union.
4. Query component sizes.
5. Measure max depth before/after repeated finds.
6. Compare optimized DSU with naive partition model.
7. Explain why connectivity stays unchanged after compression.
