# Chapter 093 — Immutable Data Structures

## Chapter Overview

**Immutable Data Structure** คือ structure ที่ state หลัง construction ไม่ถูกแก้ผ่าน API. จุดเน้นของบทนี้คือแยกคำว่า immutable ออกจาก persistent ให้ชัด:

- immutable = object นี้เปลี่ยนไม่ได้
- persistent = update แล้ว version เก่ายังเข้าถึงได้
- object อาจ immutable แต่ไม่มี update operation และจึงไม่สร้าง version chain
- persistent structure มักใช้ immutable nodes แต่แนวคิดสองอย่างไม่เท่ากัน

Implementation คือ `FrozenIntSet`: hash set แบบ build-once/read-many. Constructor copy input, deduplicate และวาง keys ใน open-addressing table. หลัง build API มีเพียง read/query ไม่มี insert/remove. เพราะไม่มี deletion จึงไม่ต้องมี tombstone และ representation ง่ายขึ้น.

## Learning Objectives

หลังจบบท ผู้เรียนต้องสามารถ:

1. นิยาม immutability ระดับ object/API
2. แยก immutable, persistent, copy-on-write และ read-only view
3. อธิบาย builder/freeze pattern
4. implement frozen open-addressing set
5. อธิบายเหตุผลที่ no-delete table ไม่ต้องมี tombstone
6. วิเคราะห์ load factor กับ probe length
7. อธิบายประโยชน์ด้าน reasoning และ thread sharing
8. ทดสอบ deduplication, negative keys, `INT_MIN/INT_MAX`
9. วิเคราะห์ storage overhead ของ keys + occupancy bytes
10. ตัดสินใจว่า workload build-once/read-many เหมาะกับ immutable representation หรือไม่

## Prerequisites

- Chapter 013 Hashing Fundamentals
- Chapter 014 Hash Table
- Chapter 092 Persistent Data Structures
- Chapter 071 Cuckoo Hashing และ 072 Perfect Hashing เป็น alternative static-set designs

## Mental Model

Mutable set:

```text
table -> insert -> delete -> tombstone -> resize -> state changes over time
```

Frozen set:

```text
input values
   |
 build + deduplicate
   v
+-------------------+
| immutable table   |
+-------------------+
    | contains
    | contains
    | contains
    v
never mutates
```

การไม่มี mutation หลัง publish ทำให้ reader หลายคนสามารถ share object เดียวกันได้โดยไม่ต้อง lock **ตราบใดที่ lifetime management ภายนอกถูกต้อง**.

## Builder / Freeze Pattern

บางระบบแยกสอง phase:

1. mutable builder สำหรับ ingest
2. `freeze()` สร้าง compact/read-optimized immutable form

บทนี้รวม builder ไว้ใน constructor เพื่อให้ ownership ชัด: input ไม่ถูกอ้างต่อ; set copy keys ลง storage ของตนเอง.

## Hash Table Representation

```text
FrozenIntSet
├── size
├── capacity = power of two
├── keys[capacity]
└── used[capacity]
```

Capacity ถูกเลือกให้ load factor ไม่เกินประมาณ 0.5 สำหรับ input count worst-case ก่อน deduplication.

Lookup:

```text
start = mix(key) & (capacity - 1)
while used[start]:
    if keys[start] == key: found
    start = (start + 1) & mask
not found
```

เพราะไม่มี deletion, empty slot แรกยืนยันได้ว่า key ไม่อยู่หลัง probe path นั้น; ไม่ต้อง distinguish never-used กับ deleted.

## Invariants

- capacity >= 8 และเป็น power of two
- occupied slot count == size
- `used[i]` เป็น 0 หรือ 1
- load factor <= 0.5
- every occupied key is discoverable by normal probe sequence
- no public operation mutates structure

## Complexity

Build คาดหวัง O(n) ภายใต้ hash distribution ที่ดี; worst-case hashing/probing สามารถ degrade ถึง O(n²) สำหรับ construction pathological. `contains` expected O(1), worst O(n). Space Θ(capacity).

Immutability ไม่ทำให้ hashing magically มี worst-case O(1); ต้องแยก semantic property ออกจาก algorithmic guarantee.

## Immutability and Concurrency

Immutable data ลด race condition ที่เกิดจาก concurrent writes เพราะไม่มี write API. แต่ยังต้องจัดการ:

- lifetime/free หลัง reader คนสุดท้าย
- publication visibility
- external mutable objects ที่ key อาจอ้างถึง (บทนี้ใช้ plain int จึงไม่มีปัญหานี้)
- allocator/thread ownership

ดังนั้นคำว่า immutable ไม่ได้แปลว่า “ทุกอย่าง thread-safe โดยอัตโนมัติ” ในทุกภาษา/runtime.

## Real-World Uses

- configuration snapshots
- routing/policy tables ที่ rebuild แล้ว swap
- compiler constant tables
- read-only dictionaries
- static lookup indexes
- cacheable build artifacts
- generated code/data tables

## When NOT to Use This Structure

- write/update เป็น workload หลัก
- ต้อง delete/insert แบบ incremental
- memory ของ duplicated rebuilds แพง
- need ordered traversal/range query
- adversarial keys ต้อง guarantee worst-case
- data เปลี่ยนบ่อยกว่าระยะเวลาที่ amortize build cost ได้

## Relation to Next Chapters

Chapter 094 จะเข้าสู่ Functional Data Structures ซึ่งใช้ immutable values เป็น default style มากขึ้น. Chapter 095 จะศึกษาการทำให้ object ดูแยกกันแต่ share backing storage จนกว่าจะเขียนด้วย Copy-on-Write.

## Build → Run → Observe → Explain

```bash
cmake -S . -B build
cmake --build build --target ch093_immutable_tests ch093_immutable_benchmark
ctest --test-dir build -R ch093 --output-on-failure
./build/ch093_immutable_benchmark
```
