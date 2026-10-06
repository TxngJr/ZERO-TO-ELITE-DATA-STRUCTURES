# Chapter 101 — Concurrent Hash Map

Chapter 097 ใช้ striped map แบบ fixed buckets. บทนี้เพิ่มความยากที่ระบบจริงต้องเจอ: **concurrent resizing** โดยยังรักษา lookup/update correctness.

Implementation ใช้สองระดับ synchronization:

1. table-level `pthread_rwlock_t` ป้องกัน bucket-array identity และ resize
2. mutex ต่อ bucket ป้องกัน linked chain ภายใน bucket

Normal get/put/remove:
```text
table read lock -> bucket mutex -> operation -> unlock bucket -> unlock table
```

Resize:
```text
table write lock -> nobody can hold table read lock
                 -> allocate new bucket array
                 -> rehash/move every node
                 -> publish new table
                 -> destroy old bucket locks
                 -> unlock
```

## Why Two Levels?

ถ้าไม่มี table lock, thread อาจคำนวณ bucket จาก array เก่า ขณะที่อีก thread rehash/free array นั้น. Bucket lock อย่างเดียวไม่สามารถป้องกัน identity/lifetime ของ bucket array ระหว่าง resize.

Read lock ช่วยให้ operations หลาย bucket ทำงานพร้อมกันได้ ส่วน resize เป็น rare global write phase.

## Resize Policy

Bucket count เป็น power of two. เมื่อ `size > 4 * bucket_count` insertion จะ trigger best-effort resize เป็น 2×.

Allocation failure ของ resize **ไม่ rollback insertion ที่สำเร็จแล้ว**: map ยัง correct แต่ performance อาจแย่ลง. Resize จึงเป็น optimization ไม่ใช่ส่วนหนึ่งของ logical put transaction.

## Atomic Size

`size` และ `resize_count` ใช้ C atomics เพื่อให้ query metrics ไม่ต้องเพิ่ม global size mutex. Linked data ยังต้องอาศัย locks.

## Tests

- initial 4 buckets
- 8 threads × 5,000 disjoint inserts
- multiple concurrent resizes
- 8-thread update pass
- exact get verification
- 8-thread even-key removal
- exact odd-key membership
- TSan

## Complexity

Expected chain work ดีเมื่อ resize ทัน:
- get/update/remove expected O(1)
- insertion expected O(1) amortized
- resize Θ(n)
- worst-case hash collision O(n)

Concurrency latency รวม rwlock/bucket contention และ global resize pauses.

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch101_concurrent_hash_map_tests
ctest --test-dir build-tsan -R "^ch101_" --output-on-failure
```
