# Chapter 110 — External-Memory Data Structures

บทนี้เปลี่ยน cost model จาก CPU operations ไปเป็น **block transfers / I/O operations**.

Implementation คือ `ExternalMemorySortedSet`:
- sorted unique uint64 values
- data packed into fixed-capacity logical blocks
- RAM-resident block directory with first/last key
- point query touches at most one data block after directory search
- range scan touches only overlapping sequential blocks
- explicit logical block-read/write counters

> Structure นี้จำลอง external-memory model ใน RAM เพื่อให้ I/O complexity วัดได้ deterministic. Chapter 111 จะย้าย representation ไปไฟล์จริง.

## I/O Model

ให้:
- N = number of keys
- B = keys per block

จำนวน data blocks ≈ ceil(N/B).

Directory search is modeled as RAM metadata. Data access increments logical block-read counters.

## Point Query

1. binary-search directory by block.last
2. if candidate block exists, count one block read
3. binary-search only that block

Thus data-block transfer cost <= 1 after directory lookup in this teaching model.

## Range Query

Find first block whose last >= low, then scan consecutive blocks until first > high. I/O cost is proportional to number of overlapping blocks.

## Tests

- 100,000 strictly increasing keys
- block capacity 128 keys
- 20,000 random membership queries vs arithmetic reference
- every point query checks logical reads <= 1
- range collection compared with dense reference
- exact build write count = block count
- ASan/UBSan
