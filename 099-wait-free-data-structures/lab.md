# Lab — Bounded Steps vs Caller Retries

1. Physical capacity 257.
2. Producer ส่ง 500,000 sequential integers.
3. Consumer verify exact order.
4. Count full/empty misses ที่ caller retry.
5. ยืนยันว่าแต่ละ `try_*` call เองไม่มี loop.
6. Run TSan.

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch099_wait_free_tests
ctest --test-dir build-tsan -R "^ch099_" --output-on-failure
```

เปลี่ยนเป็น two producers ใน isolated experiment แล้วอธิบายว่า contract ถูกละเมิด ไม่ใช่ algorithm suddenly กลายเป็น MPSC.
