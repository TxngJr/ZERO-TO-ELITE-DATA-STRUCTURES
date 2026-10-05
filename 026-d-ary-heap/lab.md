# Lab 026 — Tune the Branching Factor

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch026_dary_heap_tests

Tasks:
1. Build same array with d=2,4,8.
2. Verify identical pop-min sequence.
3. Draw first three levels for d=4.
4. Count comparisons conceptually for sift-down.
5. Benchmark d=2,4,8,16.
6. Explain which d wins on your machine and why result is workload-specific.
