# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 30 — Chapters 088–090 complete**

ล่าสุด:
- [088 Sparse Matrix Representations](./088-sparse-matrix-representations/)
- [089 Matrix/Tensor Storage Layout](./089-matrix-tensor-storage-layout/)
- [090 Compressed Data Structures](./090-compressed-data-structures/)

สถานะ: **90 / 170 chapters**

Next:

**091 Succinct Data Structures → 092 Persistent Data Structures → 093 Immutable Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
