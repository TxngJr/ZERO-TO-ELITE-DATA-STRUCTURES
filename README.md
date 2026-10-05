# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 20 — Chapters 058–060 complete**

ล่าสุด:
- [058 Spatial Hashing](./058-spatial-hashing/)
- [059 Suffix Array](./059-suffix-array/)
- [060 Suffix Tree](./060-suffix-tree/)

สถานะ: **60 / 170 chapters**

Next:

**061 Suffix Automaton → 062 Aho-Corasick Automaton → 063 Rolling Hash Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
