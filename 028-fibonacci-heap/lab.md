# Lab 028 — Pay Later with Potential

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch028_fibonacci_heap_tests

Tasks:
1. Insert many keys and observe insert requires no degree consolidation.
2. Extract min and explain why consolidation happens now.
3. Meld two heaps and verify source becomes empty.
4. Decrease a child below its parent to trigger cut.
5. Construct a marked-parent scenario and trigger cascading cut.
6. Compare repeated decrease-key workload with Binomial Heap.
7. Explain actual vs amortized cost.
