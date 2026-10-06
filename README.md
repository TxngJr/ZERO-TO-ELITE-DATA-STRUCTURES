# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 32 — Chapters 094–096 complete**

ล่าสุด:
- [094 Functional Data Structures](./094-functional-data-structures/)
- [095 Copy-on-Write Structures](./095-copy-on-write-structures/)
- [096 Concurrent Data Structures](./096-concurrent-data-structures/)

สถานะ: **96 / 170 chapters**

Next:

**097 Thread-Safe Queue / Map → 098 Lock-Free Data Structures → 099 Wait-Free Data Structures**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer for concurrent chapters:

    cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
    cmake --build build-tsan --target ch096_concurrent_demo ch096_concurrent_tests ch096_atomic_publication
    ctest --test-dir build-tsan -R "^ch096_" --output-on-failure --timeout 60
