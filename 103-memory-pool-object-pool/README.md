# Chapter 103 — Memory Pool / Object Pool

Memory/Object Pool จองพื้นที่สำหรับ object ขนาดคงที่ล่วงหน้า แล้ว reuse slots เดิมแทนการเรียก malloc/free ทุก object.

Implementation หลักคือ `ObjectPool`:
- fixed object size และ fixed capacity
- stride ถูก round up ตาม `alignof(max_align_t)`
- free-slot stack แยกจาก payload
- O(1) allocate/release
- ownership validation
- double-free rejection
- zero-fill เฉพาะ logical object bytes ตอน allocate
- bulk destroy เมื่อ pool หมดอายุ

## Mental Model

```text
storage:
+--------+--------+--------+--------+
| slot 0 | slot 1 | slot 2 | slot 3 |
+--------+--------+--------+--------+

free_stack: [3,2,1,0]
in_use:    [0,0,0,0]
```

Allocate pop index จาก free stack. Release push index กลับ โดย payload ไม่ถูกใช้เป็น free-list metadata.

## Why Separate Metadata?

Allocator บางแบบเขียน next-pointer ลง free object เองได้ แต่บทนี้แยก metadata เพื่อ:
- รองรับ object เล็กกว่า pointer
- ตรวจ double free ได้ตรงไปตรงมา
- ไม่ทำลาย payload เพื่อซ่อน allocator state
- ทำ invariant validator ได้ง่าย

## Complexity

| Operation | Time |
|---|---:|
| allocate | Θ(1) |
| release | Θ(1) |
| owns | Θ(1) |
| count | Θ(1) |
| full validate | Θ(capacity) |

Space = payload storage + free-index stack + in-use bitmap.

## Tests

- randomized **100,000 allocate/free operations**
- capacity 1,024
- every step checks live-count model
- periodic structural validation
- double-free rejection
- foreign-pointer rejection
- slot reuse check
- ASan/UBSan + LeakSanitizer

## Build

```bash
cmake -S . -B build-asan -DDS_ENABLE_SANITIZERS=ON
cmake --build build-asan --target ch103_object_pool_tests ch103_object_pool_benchmark
ctest --test-dir build-asan -R "^ch103_" --output-on-failure
```
