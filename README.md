# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research โดยใช้ Fedora Linux เป็น environment หลัก และเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 09 — Chapters 025–027 complete**

ล่าสุด:
- [025 Heap](./025-heap/)
- [026 D-ary Heap](./026-d-ary-heap/)
- [027 Binomial Heap](./027-binomial-heap/)

สถานะ: **27 / 170 chapters**

Next:

**028 Fibonacci Heap → 029 Trie / Prefix Tree → 030 Radix Tree / Patricia Trie**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure

ดู ROADMAP.md, COURSE_STATE.md, COVERAGE_MATRIX.md และ GLOSSARY.md สำหรับรายละเอียด.
