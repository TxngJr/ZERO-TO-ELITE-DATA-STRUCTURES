# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 28 — Chapters 082–084 complete**

ล่าสุด:
- [082 Circular Buffer / Ring Buffer](./082-circular-buffer-ring-buffer/)
- [083 Gap Buffer](./083-gap-buffer/)
- [084 Rope](./084-rope/)

สถานะ: **84 / 170 chapters**

Next:

**085 Piece Table → 086 Inverted Index → 087 Posting List**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

Sanitizers:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60
