# Lab 027 — Meld Forests

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch027_binomial_heap_tests

Tasks:
1. Insert one element and identify B0.
2. Insert a second and observe B0+B0 -> B1.
3. Insert through size 8 and relate root degrees to binary size.
4. Meld two heaps and verify source becomes empty.
5. Extract minimum and trace child-list reversal.
6. Decrease a handle key and trace bubbling.
7. Compare meld benchmark with reinserting values into Binary Heap.
