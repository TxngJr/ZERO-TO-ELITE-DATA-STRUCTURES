# Hands-On Lab — Measure Rank/Select Directories

## Goal
สร้าง static bit vector, ตรวจ correctness และวัดพื้นที่จริง.

## Prerequisites
Chapters 064, 066, 090.

## Task
1. Build test target.
2. Predict rank/select ของ demo ก่อนรัน.
3. รัน randomized tests.
4. รัน benchmark.
5. เปลี่ยน `SUPER_WORDS` เป็น 4 และ 16 แล้ววัด latency/bytes.

## Commands

```bash
cmake -S . -B build
cmake --build build --target ch091_succinct_tests ch091_succinct_benchmark
ctest --test-dir build -R ch091 --output-on-failure
./build/ch091_succinct_benchmark
```

## Verification
- tests ต้องผ่าน boundary 63/64/65 และ 511/512/513
- sanitizer ต้องไม่มี invalid access
- `total_structure >= packed_payload`

## Debugging Task
จงเอา final-word mask ออกจาก `select0`, เพิ่ม test size 65, observe failure แล้วอธิบาย root cause.

## Extension
เพิ่ม sampled select directory และเทียบ bytes/query time.
