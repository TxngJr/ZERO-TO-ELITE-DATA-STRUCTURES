# Lab 023 — Make Access Reshape the Tree

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch023_splay_tests

Tasks:
1. Insert 10,20,30 and watch each new key become root.
2. Build a larger tree and access a deep key.
3. Verify accessed key becomes root.
4. Repeat same access and reason why next access is cheap.
5. Search a missing key and observe last-accessed boundary splayed.
6. Delete root and inspect join operation.
7. Compare inorder before/after splaying.
