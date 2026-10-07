# Chapter 117 — AI/ML Data Structures

บทนี้เน้น data structures ที่อยู่ใน training / inference pipeline โดยยังไม่เข้าสู่ vector-search index ของ Chapter 118.

Implementation มี 3 ส่วน:

1. **MlDataset** — contiguous row-major feature matrix + labels
2. **BatchPlan** — permutation/index plan สำหรับ shuffled mini-batches
3. **EmbeddingTable** — dense embedding matrix + gather

## Dense Dataset

Features เก็บ contiguous:

```text
row 0: f0 f1 f2 ...
row 1: f0 f1 f2 ...
...
```

แต่ละ row มี initialized flag เพื่อไม่ให้ gather อ่านข้อมูลที่ยังไม่ถูก set.

## BatchPlan

Batch plan เก็บ permutation ของ row indices แยกจาก feature storage.

ข้อดี:
- shuffle โดยไม่ย้าย tensor payload
- batch เป็น view ของ indices
- epoch reset ทำได้เร็ว
- validator ตรวจว่า permutation มีทุก row exactly once

Fisher-Yates shuffle ใช้ deterministic SplitMix64 seed เพื่อ reproducibility ใน lab/tests.

## Embedding Table

Embedding table เก็บ `rows × dims` contiguous floats.
`embedding_gather` copy rows ตาม index list ไป output batch.

บทนี้ intentionally **ไม่ทำ nearest-neighbor search** เพราะ Chapter 118 จะเรียน Vector Search Structures โดยตรง.

## Tests

- dataset 50,000 rows × 16 features
- exact row/label reconstruction
- shuffled batches size 128
- every row appears exactly once per epoch
- embedding table 20,000 × 32
- gather 10,000 generated indices
- exact value verification
- ASan/UBSan

## Complexity

Dataset row access O(1).
Batch shuffle O(N), next-batch O(1) metadata.
Gather O(batch × feature/dim width).
