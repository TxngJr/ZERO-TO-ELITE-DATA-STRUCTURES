# Lab 062 — Match Many Patterns at Once

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch062_aho_corasick_tests

Tasks:
1. Build patterns he/she/his/hers.
2. Draw trie.
3. Compute failure links.
4. Compute output links.
5. Scan "ushers".
6. Record every match ID and end position.
7. Compare with naive per-pattern scanning.
