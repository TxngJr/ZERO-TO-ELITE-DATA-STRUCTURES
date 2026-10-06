# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 31 — Chapters 091–093 complete**

ล่าสุด:
- [091 Succinct Data Structures](./091-succinct-data-structures/)
- [092 Persistent Data Structures](./092-persistent-data-structures/)
- [093 Immutable Data Structures](./093-immutable-data-structures/)

สถานะ: **93 / 170 chapters**

Next:

**094 Functional Data Structures → 095 Copy-on-Write Structures → 096 Concurrent Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
