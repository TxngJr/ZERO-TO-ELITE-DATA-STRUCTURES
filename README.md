# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 05 — Chapters 013–015 complete**

1. [001 Programming Foundations](./001-programming-foundations/)
2. [002 Memory Fundamentals](./002-memory-fundamentals/)
3. [003 Abstract Data Type (ADT)](./003-abstract-data-type/)
4. [004 Complexity Analysis](./004-complexity-analysis/)
5. [005 Recursion & Iteration](./005-recursion-iteration/)
6. [006 Arrays](./006-arrays/)
7. [007 Strings](./007-strings/)
8. [008 Linked Lists](./008-linked-lists/)
9. [009 Stack](./009-stack/)
10. [010 Queue](./010-queue/)
11. [011 Deque](./011-deque/)
12. [012 Priority Queue](./012-priority-queue/)
13. [013 Hashing Fundamentals](./013-hashing-fundamentals/)
14. [014 Hash Table](./014-hash-table/)
15. [015 Hash Set / Hash Map](./015-hash-set-hash-map/)

สถานะ: **15 / 170 chapters**

Next: **016 Trees Fundamentals → 017 Binary Tree → 018 Tree Traversal**

## Repository Guide

- [COURSE_GUIDE.md](./COURSE_GUIDE.md)
- [ROADMAP.md](./ROADMAP.md)
- [STUDY_ORDER.md](./STUDY_ORDER.md)
- [COURSE_STATE.md](./COURSE_STATE.md)
- [COVERAGE_MATRIX.md](./COVERAGE_MATRIX.md)
- [GLOSSARY.md](./GLOSSARY.md)
- [CHANGELOG.md](./CHANGELOG.md)

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure

ทุกบทเน้น abstraction, representation, invariants, correctness, complexity, implementation, testing, benchmarking และ trade-offs.
