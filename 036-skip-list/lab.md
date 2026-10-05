# Lab 036 — Build Express Lanes

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch036_skip_list_tests

Tasks:
1. Insert 1..30.
2. Dump/inspect current active level.
3. Search several keys top-down.
4. Delete a tall node and observe links at every level.
5. Run range [10,20].
6. Compare membership benchmark with B-Tree.
7. Explain why Skip List is attractive as an LSM MemTable.
