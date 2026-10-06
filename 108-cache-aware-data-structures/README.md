# Chapter 108 — Cache-Aware Data Structures

บทนี้เริ่มหมวด Hardware / Storage-Aware Structures โดยใช้ **tiled matrix layout** เป็นตัวอย่างที่ควบคุม spatial locality ผ่าน knowledge ของ block/tile size โดยตรง.

Implementation คือ `CacheAwareMatrix`:
- logical rows × cols
- tile_rows × tile_cols
- tiles stored contiguously
- edge tiles padded with zeros
- O(1) logical get/set
- row-order vs tile-order traversal
- tiled transpose into dense output
- overflow-safe layout calculations

## Layout

```text
logical matrix
+----+----+----+
| T0 | T1 | T2 |
+----+----+----+
| T3 | T4 | T5 |
+----+----+----+

physical:
[T0 cells][T1 cells][T2 cells][T3 cells]...
```

Inside each tile, elements are row-major. Edge tilesมี padding เพื่อให้ทุก tile มี physical size เท่ากัน.

## Why Cache-Aware?

Layout เลือก tile dimensions อย่าง explicit. นั่นหมายถึง structure ใช้ hardware/storage block-size knowledge เป็น parameter.

Cache-aware ไม่ได้แปลว่าเร็วกว่าเสมอ:
- logical row scan บน tiled layout อาจกระโดดข้าม tiles
- tile-order workload ได้ contiguous accesses มากกว่า
- optimal tile size ขึ้นกับ cache line, cache capacity, algorithm และ element size

## Tests

- dense reference **257 × 193**
- tiles 16 × 16
- **100,000 random updates**
- every cell copied back and compared
- row-order sum = tile-order sum
- tiled transpose exact comparison
- edge padding stays zero
- ASan/UBSan

## Benchmark

Benchmark ใช้ matrix ใหญ่และวัด:
- row-order traversal บน tiled storage
- tile-order traversal บน tiled storage
- tiled transpose

ผล benchmark เป็น observation ของเครื่อง/runner ไม่ใช่ theorem ว่า tile order ต้องชนะทุกกรณี.
