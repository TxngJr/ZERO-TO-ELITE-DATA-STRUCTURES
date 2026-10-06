# Lab — CAS Retry and ABA Boundary

1. Run 8 concurrent push threads.
2. Observe push CAS failure count.
3. Run concurrent pop threads and verify values exactly once.
4. Run mixed producers/consumers.
5. Confirm pool exhaustion returns `pushed=false`.
6. Run TSan.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch098_lock_free_tests
ctest --test-dir build-tsan -R "^ch098_" --output-on-failure
```

Do not “optimize” by recycling popped slots until a safe reclamation/ABA strategy is designed.
