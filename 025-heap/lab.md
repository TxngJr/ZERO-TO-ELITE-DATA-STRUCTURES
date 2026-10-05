# Lab 025 — Heapify Instead of Reinsert

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch025_heap_tests

Tasks:
1. Build from [9,4,7,1,3,6,2].
2. Trace bottom-up sift-down order.
3. Build same values with repeated push.
4. Verify both heaps are valid even if arrays differ.
5. Pop all and verify ascending sequence.
6. Run heap sort.
7. Run construction benchmark.
