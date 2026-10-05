# Lab 019 — Build and Break a BST

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch019_bst_tests

Tasks:
1. Insert 8,3,10,1,6,14,4,7,13.
2. Predict inorder.
3. Trace search for 7 and 12.
4. Delete a leaf, one-child node and two-child node.
5. Validate after every mutation.
6. Insert sorted 1..20 and inspect height.
7. Compare with shuffled insertion benchmark.
