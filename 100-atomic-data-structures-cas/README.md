# Chapter 100 — Atomic Data Structures / CAS

บทนี้เจาะลึก atomic read/modify/write และ Compare-and-Swap (CAS) จากพื้นฐาน Chapter 096–099 โดยแยกให้ชัดว่า atomicity ของ primitive, memory ordering, linearization point และ progress guarantee เป็นคนละเรื่องกัน.

Implementation มี 2 ส่วน:

- `AtomicBitset` — packed bitset ที่ใช้ atomic fetch-or / fetch-and / fetch-xor และ word-level CAS
- `AtomicTaggedValue` — pack `value + version` ใน 64-bit atomic state เพื่อสาธิต tagged CAS และ ABA mitigation

## Learning Objectives

หลังจบบท ผู้เรียนต้องสามารถ:
1. อธิบาย load/store กับ read-modify-write atomics
2. แยก `fetch_or`, `fetch_and`, `fetch_xor` และ CAS
3. อธิบาย strong vs weak compare-exchange
4. หา linearization point ของ atomic bit update
5. อธิบาย acquire/release/acq_rel
6. เข้าใจ spurious failure ของ weak CAS
7. อธิบาย ABA และเหตุผลของ version tag
8. ตรวจ runtime lock-free status ด้วย `atomic_is_lock_free`
9. แยก atomic primitive correctness จาก object-level correctness
10. ใช้ sanitizer + contention metrics ตรวจ implementation

## AtomicBitset

Bit `i` อยู่ที่:

```text
word = i / 64
mask = 1ULL << (i % 64)
```

`test_and_set` ใช้ atomic fetch-or. Return value ของ fetch-or คือ word ก่อน update จึงบอก previous bit ได้โดยไม่ต้อง load แยก.

`compare_exchange_word` ใช้ CAS ทั้ง word และตรวจ padding ของ final word เพื่อไม่ให้ caller publish bits นอก logical size.

## AtomicTaggedValue

State:

```text
63                   32 31                    0
+----------------------+----------------------+
|      version         |        value         |
+----------------------+----------------------+
```

CAS success จะเพิ่ม version 1. ถ้าค่าเปลี่ยน A→B→A แต่ version จาก 7→8→9, stale snapshot `(A,7)` จะไม่ match `(A,9)`.

## Progress

CAS loop ใน `atv_fetch_increment` อาจ retry ไม่จำกัดสำหรับ thread หนึ่ง จึงไม่อ้าง wait-free. Progress guarantee ยังขึ้นกับว่า atomic 64-bit ของ platform เป็น lock-free จริงหรือไม่.

## Complexity

| Operation | Work |
|---|---:|
| bit test | O(1) |
| bit set/clear/toggle | O(1) atomic RMW |
| word CAS | O(1) attempt |
| tagged CAS | O(1) attempt |
| fetch increment | O(1 + retries) |
| quiescent bit count | O(words) |

## Tests

- 8 threads set 8,192 unique bits
- exact quiescent popcount
- final-word padding CAS rejection
- explicit A→B→A stale-tag test
- 8 threads × 10,000 CAS increments
- CAS retry counter

## Build

```bash
cmake -S . -B build-tsan -DDS_ENABLE_THREAD_SANITIZER=ON
cmake --build build-tsan --target ch100_atomic_tests
ctest --test-dir build-tsan -R "^ch100_" --output-on-failure
```
