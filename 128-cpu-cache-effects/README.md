# Chapter 128 — CPU Cache Effects

บทนี้ต่อจาก Chapter 127 โดยเพิ่ม **cache capacity + set mapping + associativity + LRU replacement**.

Simulator เป็น configurable set-associative cache:

- line size
- set count
- associativity
- exact LRU ordering per set

Address mapping:

```text
line = address / line_size
set  = line % set_count
tag  = line / set_count
```

## Statistics

- accesses
- hits
- misses
- evictions

ทุก accessเป็น deterministic software simulation; ไม่ขึ้นกับ CPUจริง.

## Why LRU Lists?

แต่ละ setเก็บ tagsเรียงจาก **MRU → LRU**.
Hit เลื่อน tagนั้นขึ้นหน้า.
Miss:
- ถ้ายังมี free way → insertหน้า
- ถ้าเต็ม → evictท้ายแล้ว insertหน้า

## Tests

- exact direct-mapped conflict trace
- exact 2-way LRU trace
- reset semantics
- invalid configuration rejection
- row-major vs column-major 64×64 matrix trace under constrained cache
- invariant validation after thousands of accesses
- ASan/UBSan

## Important

นี่คือ teaching simulator ไม่ใช่ modelของ CPUรุ่นใดรุ่นหนึ่ง:
- ไม่มี prefetcher
- ไม่มี write policy
- ไม่มี multi-level hierarchy
- ไม่มี coherence
- replacement policy fixedเป็น LRU
