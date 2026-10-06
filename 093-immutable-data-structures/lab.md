# Hands-On Lab — Build Once, Read Many

## Goal
ทดลองว่า semantic constraint “ไม่มี update” ทำให้ hash table ง่ายลงอย่างไร.

## Task
1. Build set ที่มี duplicates.
2. ตรวจ deduped size.
3. Query hit/miss จำนวนมาก.
4. วัด capacity/load/storage.
5. เปรียบเทียบกับ mutable hash table chapter 014 ในเชิง representation.

## Commands

```bash
cmake -S . -B build
cmake --build build --target ch093_immutable_tests ch093_immutable_benchmark
ctest --test-dir build -R ch093 --output-on-failure
./build/ch093_immutable_benchmark
```

## Verification
- `INT_MIN`, `INT_MAX`, duplicates ต้องถูกต้อง
- load <= 0.5
- missing keys ต้องไม่ false-positive

## Debugging Task
เปลี่ยน capacity เป็น non-power-of-two แต่ยังใช้ bit mask แล้ว observe invariant/query failures.

## Extension
เพิ่ม metrics ของ average/max probe distance.
