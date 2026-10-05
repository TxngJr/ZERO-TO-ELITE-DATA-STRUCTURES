# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 16 — Chapters 046–048 complete**

ล่าสุด:
- [046 Persistent Segment Tree](./046-persistent-segment-tree/)
- [047 Fenwick Tree / Binary Indexed Tree](./047-fenwick-tree/)
- [048 Sparse Table](./048-sparse-table/)

สถานะ: **48 / 170 chapters**

Next:

**049 Interval Tree → 050 Interval Heap → 051 Range Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
