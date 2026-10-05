# Lab 053 — Build with a Monotonic Stack

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch053_cartesian_tree_tests

Tasks:
1. Build [3,1,4,0,2] by hand.
2. Record stack after each input.
3. Draw parent/left/right arrays.
4. Verify inorder indices.
5. Query [1,4).
6. Build ascending input and measure height.
7. Explain RMQ <-> LCA.
