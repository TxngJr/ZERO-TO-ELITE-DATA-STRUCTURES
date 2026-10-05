# Lab 021 — Make BST Stay Short

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch021_avl_tests

Tasks:
1. Insert 30,20,10 and identify LL.
2. Insert 10,20,30 and identify RR.
3. Build LR and RL sequences.
4. Insert 1..100 and inspect height.
5. Remove many keys and validate after each removal.
6. Compare inorder against sorted reference set.
7. Run benchmark against Chapter 019 plain BST.
