# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 19 — Chapters 055–057 complete**

ล่าสุด:
- [055 Quadtree](./055-quadtree/)
- [056 Octree](./056-octree/)
- [057 R-Tree](./057-r-tree/)

สถานะ: **57 / 170 chapters**

Next:

**058 Spatial Hashing → 059 Suffix Array → 060 Suffix Tree**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
