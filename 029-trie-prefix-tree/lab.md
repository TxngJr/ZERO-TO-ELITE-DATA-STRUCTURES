# Lab 029 — Search by Prefix

Build/test:

    cmake -S . -B build
    cmake --build build
    ./build/ch029_trie_tests

Tasks:
1. Insert app, apple, apt, cat.
2. Compare contains("ap") vs has_prefix("ap").
3. Count keys under prefix "ap".
4. Delete "app" and ensure "apple" survives.
5. Visit all keys and verify lexicographic order.
6. Insert a binary key containing zero byte.
7. Compare prefix workload with linear string scanning benchmark.
