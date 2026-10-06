# Chapter 111 — Disk-Based Data Structures

บทนี้ย้ายจาก I/O model ของ Chapter 110 ไปสู่ **persistent file representation จริง**.

Implementation คือ `DiskSortedIndex`:
- persistent sorted uint64 key → int64 value records
- portable little-endian file encoding
- 64-byte header
- fixed 16-byte records
- create → close → reopen
- binary-search point lookup with random seeks
- lower_bound + sequential range scan
- file-size/header/order validation
- logical seek + record-read counters

## File Format

```text
64-byte header
+---------------------------+
| magic "ZDSIDX11"          | 8
| version = 1               | 8 LE
| record_count              | 8 LE
| record_size = 16          | 8 LE
| reserved zeros            | 32
+---------------------------+

records:
+----------------+----------------+
| key uint64 LE  | value int64 LE |
+----------------+----------------+
```

Input keys must be strictly increasing.

## Point Lookup

Binary search reads records by index:

```text
offset = 64 + index * 16
fseeko
fread 16 bytes
```

This exposes random-access behavior instead of pretending file access has RAM cost.

## Range Scan

1. binary-search lower_bound(low)
2. one seek to first candidate
3. sequential fread records until key > high

This demonstrates the important disk pattern: use an index/search to find the start, then prefer sequential I/O.

## Tests

- create **50,000 records**
- close and reopen file
- full structural validation
- 20,000 random point lookups
- exact values
- range scan exact comparison
- missing/corrupt header rejection
- temporary file cleanup
- ASan/UBSan
