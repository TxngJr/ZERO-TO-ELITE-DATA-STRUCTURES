# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 12 — Chapters 034–036 complete**

ล่าสุด:
- [034 B* Tree](./034-b-star-tree/)
- [035 LSM Tree](./035-lsm-tree/)
- [036 Skip List](./036-skip-list/)

สถานะ: **36 / 170 chapters**

Next:

**037 Disjoint Set / Union-Find → 038 Graph Fundamentals → 039 Graph Representations**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure
