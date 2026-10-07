# Chapter 112 — Database Index Structures

บทนี้เชื่อม B/B+ Tree, disk storage และ indexing concepts เข้ากับ database-style access paths.

Implementation คือ `DbIndex` แบบ build-once/read-many ที่สร้าง index สองชุดจาก rows เดียวกัน:

- **Primary index** — sorted by unique `primary_key`
- **Secondary index** — sorted by `(secondary_key, primary_key)`
- duplicate secondary keys allowed
- point lookup by primary key
- exact secondary-key lookup
- secondary range scan
- covering metadata: secondary entries point back to rows in the primary array

## Why Two Indexes?

Table row อาจมี primary key ที่ unique แต่ query จริงมัก filter/order ด้วย column อื่น.

```text
rows:
(pk=10, sk=7)
(pk=20, sk=4)
(pk=30, sk=7)

primary index:
10 -> row
20 -> row
30 -> row

secondary index:
(4,20)
(7,10)
(7,30)
```

การเรียง secondary ด้วย primary เป็น tie-breaker ทำให้ duplicate secondary keys deterministic.

## Tests

- 50,000 unsorted input rows
- unique primary keys
- duplicated secondary keys
- 20,000 randomized primary lookups
- secondary equality/range queries compared against dense reference
- validator checks both indexes and row back-references
- ASan/UBSan
