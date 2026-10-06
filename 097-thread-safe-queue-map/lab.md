# Lab — Blocking Queue + Striped Map

Queue:
- 4 producers × 5,000 unique values
- 4 consumers
- close หลัง producers จบ
- verify ทุก value ถูก consume exactly once

Map:
- 8 threads insert disjoint key ranges
- remove even keys concurrently
- verify exact odd membership

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch097_thread_safe_tests
ctest --test-dir build-tsan -R "^ch097_" --output-on-failure
```
