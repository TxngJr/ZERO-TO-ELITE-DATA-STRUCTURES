# Chapter 048 — Sparse Table

## Goal

Sparse Table เป็น static range-query structure สำหรับข้อมูลที่ **ไม่เปลี่ยนหลัง build**.

จุดเด่น:
- preprocessing Theta(n log n)
- query O(1) สำหรับ idempotent operations เช่น:
  - min
  - max
  - gcd

บทนี้ implement Range Minimum Query (RMQ):

    range_min(left,right)

บน half-open interval:

    [left,right)

## Static Contract

หลังสร้าง Sparse Table:
- ไม่มี update API
- input ถูกมองเป็น immutable snapshot

ถ้าต้อง update:
- Segment Tree/Fenwick เหมาะกว่า
- หรือ rebuild Sparse Table ใหม่

## Power-of-Two Blocks

Sparse Table stores answers for intervals:

    length = 2^k

Table entry:

    table[k][i]

represents:

    min of [i, i+2^k)

Example:

    k=0 -> length 1
    k=1 -> length 2
    k=2 -> length 4
    k=3 -> length 8

## Build Recurrence

Base:

    table[0][i] = a[i]

For k>0:

    table[k][i]
      = min(
          table[k-1][i],
          table[k-1][i + 2^(k-1)]
        )

Each 2^k block is composed from two adjacent 2^(k-1) blocks.

## Query Trick

For query [l,r):

    len = r-l
    k = floor(log2(len))
    block = 2^k

Use two blocks:

    [l, l+block)
    [r-block, r)

These blocks may overlap.

Then:

    answer = min(
        table[k][l],
        table[k][r-block]
    )

## Why Overlap Is Safe

For min:

    min(x,x) = x

This is **idempotence**.

If the two selected blocks overlap, repeated elements do not change the answer.

The same works for:
- max
- gcd
- bitwise AND
- bitwise OR

But not for ordinary sum:

    x+x != x

Therefore the two-overlapping-block O(1) trick is not valid for generic non-idempotent operations.

## Associative vs Idempotent

Segment Tree requires mainly:

    associative combine

Sparse Table O(1) overlapping RMQ requires:

    associative + idempotent

This distinction is important.

For associative but non-idempotent operations, Sparse Table can still answer by decomposing into disjoint blocks, but query becomes O(log n) unless using another technique such as Disjoint Sparse Table.

## floor(log2)

Query needs:

    k = floor(log2(len))

Teaching implementation precomputes:

    logs[len]

for every length 1..n.

Thus query avoids runtime logarithm calls.

## Memory Layout

Flattened allocation:

    table[level * n + index]

Levels:

    floor(log2(n)) + 1

Space:

    Theta(n log n)

Compared with Segment Tree:
- Sparse Table uses more memory asymptotically
- but static RMQ becomes O(1)

## Build Complexity

At level k, about:

    n - 2^k + 1

entries are built.

Across O(log n) levels:

    Theta(n log n)

## Query Complexity

Operations:
- compute len
- read logs[len]
- read two table entries
- min

Therefore:

    Theta(1)

## Validation

Validator checks:
1. log table recurrence
2. level 0 consistency
3. every valid higher-level entry satisfies recurrence
4. every queried table offset stays in allocation bounds

## Sparse Table vs Segment Tree

Sparse Table:
- static only
- build Theta(n log n)
- RMQ O(1)
- space Theta(n log n)

Segment Tree:
- updates O(log n)
- query O(log n)
- build Theta(n)
- space Theta(n)

Choose based on whether data changes.

## Sparse Table vs Fenwick Tree

Fenwick:
- point updates
- prefix/range sums
- O(log n)
- Theta(n) memory

Sparse Table:
- immutable
- idempotent RMQ
- O(1) query
- Theta(n log n) memory

Different workloads, different structures.

## Complexity

| Operation | Complexity |
|---|---:|
| build | Theta(n log n) |
| range min | Theta(1) |
| storage | Theta(n log n) |
| update | unsupported |

## Files

- src/int_sparse_table.*
- tests/test_sparse_table.c
- examples/sparse_table_demo.c
- benchmarks/sparse_table_benchmark.c
- standard learning artifacts
