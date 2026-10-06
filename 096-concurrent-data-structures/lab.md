# Lab — From Race to Linearizable Set

1. 8 threads insert disjoint ranges.
2. ตรวจ final size.
3. 8 threads remove evens.
4. ตรวจ exact odd membership.
5. Duplicate inserters พร้อมกัน.
6. Snapshot แล้วตรวจ sorted unique.
7. Run ThreadSanitizer.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch096_concurrent_tests ch096_atomic_publication
ctest --test-dir build-tsan -R ch096 --output-on-failure
```

Fedora GCC ThreadSanitizer runtime ใช้ package `libtsan` เมื่อต้องติดตั้ง runtime แยก.

Debugging exercise: ใน isolated lab copy ให้เอา lock ออกจาก contains, observe TSan race, แล้ว restore lock.
