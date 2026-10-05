# Lab 057 — Watch MBRs Grow

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch057_r_tree_tests

Tasks:
1. Insert rectangles into four spatial clusters.
2. Track least-enlargement child choices.
3. Trigger a leaf split.
4. Compute quadratic seed waste by hand.
5. Trigger root split.
6. Query a rectangle crossing two MBRs.
7. Compare results with naive rectangle scan.
