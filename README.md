# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 25 — Chapters 073–075 complete**

ล่าสุด:
- [073 Consistent Hashing](./073-consistent-hashing/)
- [074 Probabilistic Data Structures](./074-probabilistic-data-structures/)
- [075 HyperLogLog](./075-hyperloglog/)

สถานะ: **75 / 170 chapters**

Next:

**076 Count-Min Sketch → 077 Count Sketch → 078 Reservoir Sampling Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
