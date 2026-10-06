# Lab — Resize Under Concurrency

1. Start with 4 buckets.
2. 8 threads insert 40,000 disjoint keys.
3. Confirm bucket count grows.
4. Update all values concurrently.
5. Remove all even keys.
6. Verify every odd key/value exactly.
7. Run TSan.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch101_concurrent_hash_map_tests ch101_concurrent_hash_map_benchmark
ctest --test-dir build-tsan -R "^ch101_" --output-on-failure
```
