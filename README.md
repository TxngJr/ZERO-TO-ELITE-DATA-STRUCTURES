# Zero to Elite Data Structures

หลักสูตร Data Structures แบบลงลึกจากศูนย์ไปจนถึงระดับ Systems / Database / Concurrent / Research บน Fedora Linux โดยเน้น **Predict → Build → Run → Observe → Measure → Explain**.

## Current Progress

**Batch 34 — Chapters 100–102 complete**

ล่าสุด:
- [100 Atomic Data Structures / CAS](./100-atomic-data-structures-cas/)
- [101 Concurrent Hash Map](./101-concurrent-hash-map/)
- [102 Concurrent Queue / Ring Buffer](./102-concurrent-queue-ring-buffer/)

สถานะ: **102 / 170 chapters**

Next:

**103 Memory Pool / Object Pool → 104 Arena Allocator → 105 Slab Allocator Concepts**

## Build

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure --timeout 60

ASan/UBSan:

    cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
    cmake --build build-asan
    ctest --test-dir build-asan --output-on-failure --timeout 60

ThreadSanitizer for concurrent chapters 096–102:

    cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
    cmake --build build-tsan --parallel 2 --target ch096_concurrent_demo ch096_concurrent_tests ch096_atomic_publication ch097_thread_safe_demo ch097_thread_safe_tests ch098_lock_free_demo ch098_lock_free_tests ch099_wait_free_demo ch099_wait_free_tests ch100_atomic_demo ch100_atomic_tests ch101_concurrent_hash_map_demo ch101_concurrent_hash_map_tests ch102_concurrent_ring_demo ch102_concurrent_ring_tests
    ctest --test-dir build-tsan -R "^ch(09[6-9]|10[0-2])_" --output-on-failure --timeout 60
