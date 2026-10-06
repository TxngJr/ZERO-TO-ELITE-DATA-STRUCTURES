# Lab — CAS Contention and ABA

1. Run 8 threads × 10,000 tagged increments.
2. Record CAS retries.
3. Repeat with 1/2/4/8 threads.
4. Demonstrate 7/0 → 9/1 → 7/2.
5. Attempt stale CAS with expected 7/0.
6. Confirm it fails.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch100_atomic_tests ch100_atomic_benchmark
ctest --test-dir build-tsan -R "^ch100_" --output-on-failure
```
