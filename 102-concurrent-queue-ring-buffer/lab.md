# Lab — MPMC Sequence Ring

1. Create capacity 1024.
2. Start 4 consumers.
3. Start 4 producers × 25,000 unique values.
4. Verify every value exactly once.
5. Confirm final size = 0.
6. Run TSan.
7. Compare throughput to Chapter 097 mutex queue and Chapter 099 SPSC ring.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch102_concurrent_ring_tests ch102_concurrent_ring_benchmark
ctest --test-dir build-tsan -R "^ch102_" --output-on-failure
```
