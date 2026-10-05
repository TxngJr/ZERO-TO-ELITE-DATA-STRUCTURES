# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 08 — Chapters 022–024 complete**

Completed through:
- 019 Binary Search Tree
- 020 Balanced BST
- 021 AVL Tree
- [022 Red-Black Tree](./022-red-black-tree/)
- [023 Splay Tree](./023-splay-tree/)
- [024 Treap](./024-treap/)

สถานะ: **24 / 170 chapters**

Next: **025 Heap → 026 D-ary Heap → 027 Binomial Heap**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure

ดู ROADMAP.md, COURSE_STATE.md, COVERAGE_MATRIX.md และ GLOSSARY.md สำหรับรายละเอียด.
