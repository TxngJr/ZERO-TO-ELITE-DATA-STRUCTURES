# Chapter 124 — Knowledge Graph Representation

บทนี้สร้าง **triple-store knowledge graph** ที่แยก lexical terms ออกจาก triple indexes.

โครงสร้างหลัก:

- string term dictionary
- stable `KgTermId` (เริ่มที่ 1; 0 = wildcard)
- triple table `(subject, predicate, object)`
- sorted **SPO** index
- sorted **POS** index
- sorted **OSP** index
- pattern query ที่เลือก index จาก bound term แรกที่เหมาะสม

## Why Three Indexes?

คำถาม knowledge graph มัก bind คนละตำแหน่ง:

- subject bound → SPO
- predicate bound → POS
- object bound → OSP

เช่น:

```text
(Alice, knows, ?)
(?, type, Engineer)
(?, ?, Bangkok)
```

การมี permutation indexes ลดการ scan ทั้ง triple table สำหรับ query ที่มี prefix bound.

## Versioned Indexes

การเพิ่ม term ไม่ทำให้ triple index stale แต่การเพิ่ม triple จะเพิ่ม graph version.
Query ต้องใช้ indexes ที่ build จาก version ล่าสุด.

## Tests

- 10,000 interned terms
- 100,000 triples
- duplicate term interning
- duplicate triple rejection
- exact subject/predicate/object pattern counts
- exact fully-bound membership query
- stale-index rejectionหลัง mutation
- rebuild + cross-index validation
- ASan/UBSan

## Complexity

Term lookup expected O(1).
Triple insertion expected O(1) duplicate-key hash check.
Index build O(T log T).
Bound-prefix query O(log T + matches + residual filtering).
