# Lab 024 — Two Orders, One Tree

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch024_treap_tests

Tasks:
1. Insert explicit pairs:
   (50,80), (30,40), (70,20).
2. Predict rotations and root.
3. Verify inorder is key-sorted.
4. Insert equal priorities and apply key tie-break.
5. Delete root and trace merge.
6. Insert sorted keys using default random priorities.
7. Compare height against plain BST.
