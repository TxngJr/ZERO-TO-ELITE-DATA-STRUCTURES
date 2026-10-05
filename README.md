# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 07 — Chapters 019–021 complete**

1. 001 Programming Foundations
2. 002 Memory Fundamentals
3. 003 Abstract Data Type
4. 004 Complexity Analysis
5. 005 Recursion & Iteration
6. 006 Arrays
7. 007 Strings
8. 008 Linked Lists
9. 009 Stack
10. 010 Queue
11. 011 Deque
12. 012 Priority Queue
13. 013 Hashing Fundamentals
14. 014 Hash Table
15. 015 Hash Set / Hash Map
16. 016 Trees Fundamentals
17. 017 Binary Tree
18. 018 Tree Traversal
19. [019 Binary Search Tree](./019-binary-search-tree/)
20. [020 Balanced BST](./020-balanced-bst/)
21. [021 AVL Tree](./021-avl-tree/)

สถานะ: **21 / 170 chapters**

Next: **022 Red-Black Tree → 023 Splay Tree → 024 Treap**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure

ดู ROADMAP.md, COURSE_STATE.md, COVERAGE_MATRIX.md และ GLOSSARY.md สำหรับภาพรวม.
