# Chapter 035 — LSM Tree

## Goal

LSM Tree (Log-Structured Merge Tree) เปลี่ยนแนวคิดจาก page tree ที่แก้ข้อมูล in-place ไปเป็น:

1. รับ writes ลง mutable memory structure
2. flush เป็น immutable sorted runs
3. reads ตรวจข้อมูลจาก newest ไป oldest
4. compaction merge runs เพื่อควบคุม read amplification / obsolete versions

บทนี้สร้าง **Mini Int LSM** แบบ in-memory เพื่อแยก algorithmic structure ออกจาก filesystem/crash-recovery complexity.

## What This Implementation Models

- sorted MemTable
- immutable sorted SSTable-like runs
- monotonically increasing sequence numbers
- newest-write-wins
- tombstones
- flush
- full-run compaction
- point lookup
- materialized range scan
- read/run-count statistics

สิ่งที่ยัง intentionally ไม่ทำ:
- WAL
- real files / blocks
- Bloom Filters
- block index
- checksums
- manifest/version set
- leveled/tiered multi-level policy
- background compaction
- crash recovery

หัวข้อเหล่านี้กลับมาใน Chapters 112, 154 และ 166.

## MemTable

Teaching MemTable ใช้ sorted dynamic array ขนาดจำกัด.

แต่ละ entry:

    key
    value
    tombstone
    sequence

ข้อดี:
- deterministic
- binary search ง่าย
- flush ออก sorted run ได้ทันที

ข้อเสีย:
- insert ใหม่ O(M) เพราะ shift array

Real systems มักใช้:
- Skip List
- balanced tree
- concurrent ordered structure
- arena allocation

Chapter 036 จะสร้าง Skip List ต่อทันที.

## Sequence Number

ทุก mutation ใหม่ได้:

    sequence++

ถ้า key เดียวกันมีหลาย versions:
version ที่ sequence มากที่สุดชนะ.

นี่คือ core idea ของ versioned LSM visibility.

## Put

ถ้า key อยู่ใน MemTable:
- overwrite value/tombstone
- update sequence

ถ้าเป็น key ใหม่และ MemTable เต็ม:
- flush ก่อน
- แล้ว insert sorted

Logical size เพิ่มเฉพาะเมื่อ key ก่อนหน้ามองไม่เห็น.

## Delete / Tombstone

การ delete ไม่จำเป็นต้องตามไปแก้ immutable runs เก่า.

สร้าง:

    tombstone(key, newer_sequence)

Read เจอ tombstone ก่อน version เก่า:
    key considered absent

นี่คือเหตุผลที่ tombstone สำคัญกับ immutable storage.

## Flush

MemTable ที่ sorted อยู่แล้วถูก copy เป็น immutable run.

New run ถูกวางหน้า run list:

    runs[0] = newest
    runs[1] = older
    ...

MemTable clear.

Teaching modelเรียก run ว่า SSTable-like run แม้ยังอยู่ใน memory.

## Point Lookup

Order:

1. MemTable
2. runs newest → oldest

Binary search ภายในแต่ละ sorted run.

ถ้าเจอ:
- normal entry -> return value
- tombstone -> stop and report absent

ถ้ามี R runs:

    O(log M + R log S)

โดยประมาณ.

นี่คือ **read amplification**: point read อาจต้อง probe หลาย runs.

Bloom Filters และ leveled organization ช่วยลด cost ใน production.

## Compaction

บทนี้ compact **all immutable runs** เป็น run เดียว.

Process:
1. gather entries
2. sort by key ascending, sequence descending
3. for each key keep newest version
4. drop tombstone เพราะ all older immutable history is included
5. replace old runs atomically after successful allocation

MemTable ไม่รวมใน compaction และถือว่า newer กว่า runs.

## Why Dropping Tombstones Is Safe Here

Full-run compaction includes every immutable older version.

ถ้า newest immutable version ของ key เป็น tombstone:
- ไม่มี older run เหลือให้มันต้อง shadow หลัง compaction
- จึง drop tombstone ได้

ใน multi-level real LSM:
tombstone จะ drop ได้ก็ต่อเมื่อแน่ใจว่าไม่มี older version ใน lower levels/snapshots ที่ยังต้อง shadow.

## Read Amplification

ก่อน compaction:
    read may probe many runs

หลัง full compaction:
    at most one immutable run plus MemTable

## Write Amplification

Compaction rewrites data.

แม้ writes เข้า MemTable ถูก:
data อาจถูกเขียน/ย้ายหลายครั้งข้าม levels.

นี่คือ **write amplification**.

LSM design คือการ trade:
- cheap sequential/batched writes
- against compaction work and read amplification

## Space Amplification

Obsolete versions/tombstones may coexist before compaction.

Storage temporarily exceeds logical live dataset size.

## Range Scan

Teaching scan:
- materializes MemTable + all runs
- sorts by key/sequence
- resolves newest visible version
- emits [low, high]

นี่เน้น correctness.

Production LSM scan มักใช้:
- k-way merge iterator
- per-SSTable iterators
- snapshot sequence
- block cache/prefetch

## LSM vs B+ Tree

B+ Tree:
- page-oriented in-place updates
- ordered leaf chain
- strong point/range behavior
- random page write concerns

LSM:
- buffered writes
- immutable runs
- compaction
- excellent write throughput patterns
- read/write/space amplification trade-offs

Chapter 149 เปรียบเทียบ B-Tree vs LSM อย่างเป็นระบบ.

## Compaction Policies Preview

### Size-tiered
รวม runs ขนาดใกล้กัน.

### Leveled
แต่ละ level มี size budget และ key-range overlap constraints.

### Universal / hybrid
policy อื่น ๆ ตาม engine.

Policy เปลี่ยน:
- write amplification
- read amplification
- space amplification
- compaction burst behavior

## Complexity — Teaching Model

Let:
- M = MemTable capacity
- R = run count
- S = typical run size
- N = total immutable entries

| Operation | Cost |
|---|---:|
| MemTable lookup | O(log M) |
| new MemTable insert | O(M) array shift |
| point read | O(log M + R log S) |
| flush | Theta(M) copy |
| full compaction | O(N log N) teaching implementation |
| materialized range scan | O(N log N) |
| storage | live + obsolete versions before compaction |

Production merge-compaction can be linear in total sorted input size rather than O(N log N).

## Files

- src/int_lsm_tree.*
- tests/test_lsm_tree.c
- examples/lsm_demo.c
- benchmarks/read_amplification_benchmark.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
