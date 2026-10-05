# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 18 — Chapters 052–054 complete**

ล่าสุด:
- [052 Order Statistic Tree](./052-order-statistic-tree/)
- [053 Cartesian Tree](./053-cartesian-tree/)
- [054 KD-Tree](./054-kd-tree/)

สถานะ: **54 / 170 chapters**

Next:

**055 Quadtree → 056 Octree → 057 R-Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
