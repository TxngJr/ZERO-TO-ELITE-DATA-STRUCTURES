# Lab 032 — Split, Borrow, Merge

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch032_btree_tests

Tasks:
1. Create t=2 tree.
2. Insert 1..20 and mark every root/child split.
3. Delete keys to trigger borrow-left.
4. Delete keys to trigger borrow-right.
5. Trigger merge.
6. Observe root shrink.
7. Repeat with t=8 and compare height.
