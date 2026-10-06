# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 33 — Chapters 097–099 complete**

ล่าสุด:
- [097 Thread-Safe Queue / Map](./097-thread-safe-queue-map/)
- [098 Lock-Free Data Structures](./098-lock-free-data-structures/)
- [099 Wait-Free Data Structures](./099-wait-free-data-structures/)

สถานะ: **99 / 170 chapters**

Next:

**100 Atomic Data Structures / CAS → 101 Concurrent Hash Map → 102 Concurrent Queue / Ring Buffer**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer for concurrent chapters 096–099:

    cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
    cmake --build build-tsan --parallel 2 --target ch096_concurrent_demo ch096_concurrent_tests ch096_atomic_publication ch097_thread_safe_demo ch097_thread_safe_tests ch098_lock_free_demo ch098_lock_free_tests ch099_wait_free_demo ch099_wait_free_tests
    ctest --test-dir build-tsan -R "^ch09[6-9]_" --output-on-failure --timeout 60
