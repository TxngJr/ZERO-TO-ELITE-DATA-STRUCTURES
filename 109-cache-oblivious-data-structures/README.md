# Chapter 109 — Cache-Oblivious Data Structures

Chapter 108 ใช้ tile size แบบ explicit. บทนี้เปลี่ยนเป็น **Morton / Z-order matrix layout** ที่ไม่รับ cache-line size, cache capacity หรือ tile dimensions เป็น parameter.

Implementation คือ `CacheObliviousMatrix`:
- logical rows × cols
- padded square side = next power of two
- Morton/Z-order physical index
- O(1) logical get/set
- row-order traversal
- physical Z-order traversal
- dense copy and transpose
- padding invariant

## Why Cache-Oblivious?

Representation ไม่รู้ค่า B (block/cache-line size) หรือ M (cache capacity). Morton order recursively groups nearby 2D regions so locality exists across several scales.

> Cache-oblivious ไม่ได้แปลว่าไม่มี cache. หมายถึง algorithm/layout ไม่ hard-code cache parameters.

## Representation

For a power-of-two padded square, bits ของ row/column ถูก interleave:

```text
row bits: r2 r1 r0
col bits: c2 c1 c0

Morton:   r2 c2 r1 c1 r0 c0
```

Implementation interleave least-significant bits into even/odd Morton positions.

## Tests

- logical matrix 257 × 193
- padded side 512
- 100,000 random updates
- compare every logical cell with dense reference
- row-order sum = Z-order physical sum
- transpose exact comparison
- padding cells stay zero
- ASan/UBSan
