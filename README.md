# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 17 — Chapters 049–051 complete**

ล่าสุด:
- [049 Interval Tree](./049-interval-tree/)
- [050 Interval Heap](./050-interval-heap/)
- [051 Range Tree](./051-range-tree/)

สถานะ: **51 / 170 chapters**

Next:

**052 Order Statistic Tree → 053 Cartesian Tree → 054 KD-Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
