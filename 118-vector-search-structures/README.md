# Chapter 118 — Vector Search Structures

บทนี้ต่อจาก embedding table ของ Chapter 117 ด้วย **exact flat vector index** และ **top-k candidate heap**.

Implementation รองรับ:
- contiguous float vectors
- auto-generated stable vector IDs
- precomputed L2 norms
- exact L2 search
- exact cosine-distance search
- bounded max-heap ขนาด k สำหรับ top-k
- deterministic tie-break ด้วย vector ID
- finite-value / non-zero-norm validation

## Why Flat Index First?

ก่อนเรียน approximate nearest-neighbor (ANN) ต้องมี exact baseline ที่ตอบได้ถูก 100% เพื่อใช้เป็น reference สำหรับ recall/quality measurement.

Search ทุก vector แล้วเก็บเฉพาะ k candidate ที่ดีที่สุด:

```text
scan vector -> distance
                  |
                  v
             max-heap(k)
                  |
             worst at root
```

ถ้า candidate ใหม่ดีกว่า root ให้ replace แล้ว sift-down.

## Metrics

L2 squared distance:

`sum((q_i - x_i)^2)`

Cosine distance:

`1 - dot(q,x)/(||q|| ||x||)`

Index จงใจ reject zero-norm vectors เพื่อให้ cosine semantics ไม่กำกวม.

## Tests

- 20,000 vectors × 8 dimensions
- 100 exact L2 queries
- independent full-sort reference
- exact top-10 IDs/distances
- cosine self-match
- deterministic tie ordering
- invalid NaN/zero-vector rejection
- ASan/UBSan

## Complexity

Build append: O(D) per vector.
Exact query: O(ND + N log k) with bounded heap; k is normally much smaller than N.
