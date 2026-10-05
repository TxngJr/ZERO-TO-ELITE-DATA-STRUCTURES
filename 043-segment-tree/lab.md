# Lab 043 — Build, Query, Update

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch043_segment_tree_tests

Tasks:
1. Build [5,2,7,1,6,3].
2. Draw interval tree.
3. Query sum/min on [1,5).
4. Point-set index 3 to 10.
5. Recompute only the affected ancestor path by hand.
6. Compare 10,000 random operations with a naive array.
7. Explain why build is Theta(n).
