# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 24 — Chapters 070–072 complete**

ล่าสุด:
- [070 Cuckoo Filter](./070-cuckoo-filter/)
- [071 Cuckoo Hashing](./071-cuckoo-hashing/)
- [072 Perfect Hashing](./072-perfect-hashing/)

สถานะ: **72 / 170 chapters**

Next:

**073 Consistent Hashing → 074 Probabilistic Data Structures → 075 HyperLogLog**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
