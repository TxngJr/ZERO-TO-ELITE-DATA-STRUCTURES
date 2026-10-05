# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 15 — Chapters 043–045 complete**

ล่าสุด:
- [043 Segment Tree](./043-segment-tree/)
- [044 Lazy Propagation Segment Tree](./044-lazy-propagation-segment-tree/)
- [045 Dynamic Segment Tree](./045-dynamic-segment-tree/)

สถานะ: **45 / 170 chapters**

Next:

**046 Persistent Segment Tree → 047 Fenwick Tree / Binary Indexed Tree → 048 Sparse Table**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
