# Lab — First-Write Cost

สร้าง vector 1,000,000 ints, clone 2,000 handles, วัด clone time แล้ว mutate 100 clones และวัด detach time. ตรวจ original ไม่เปลี่ยนและรัน LeakSanitizer.

```bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan --target ch095_cow_tests ch095_cow_benchmark
ctest --test-dir build-asan -R ch095 --output-on-failure
```

Debugging: ตัด free เมื่อ refs→0 ออกจาก detach แล้วสังเกต LeakSanitizer report.
