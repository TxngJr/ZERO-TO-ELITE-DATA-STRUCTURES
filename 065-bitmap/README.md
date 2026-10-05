# Chapter 065 — Bitmap

## Goal

Bitmap ใช้ bit positions แทนสมาชิกของ integer universe แบบ dense.

กำหนด universe:

    [0, U)

Integer x เป็นสมาชิกเมื่อ bit x = 1.

บทนี้ implement mutable dense bitmap พร้อม:
- add / remove / contains
- cached cardinality
- set_range / clear_range
- union / intersection / difference
- next-member iteration
- validation

## Why Bitmap?

ถ้า universe มีขนาดจำกัดและค่อนข้าง dense:

    U bits

สามารถแทน set ของ integers ได้ตรง ๆ.

ตัวอย่าง:
- user IDs ใน bounded shard
- allocated/free slots
- page occupancy
- permission flags จำนวนมาก
- database bitmap index
- visited/active IDs

## Bitmap vs Bitset

Representation อาจเหมือนกันมาก แต่ abstraction ต่างกัน.

Bitset:
- vector of boolean positions
- boolean algebra

Bitmap:
- set of integers from a universe
- membership/cardinality/range/set algebra/iteration

ชื่อในโลกจริงมัก overlap กัน แต่การคิด abstraction ช่วยเลือก API ให้เหมาะ.

## Cached Cardinality

Bitmap เก็บ:

    cardinality

จึงตอบจำนวนสมาชิก O(1).

Mutation ต้อง update อย่างแม่นยำ:
- add only increments when 0 -> 1
- remove only decrements when 1 -> 0
- range operations use popcount before/after word masks
- set algebra recomputes result cardinality

Trade-off:
- mutation slightly more work
- cardinality query cheapมาก

## Range Mutation

API uses half-open integer range:

    [begin, end)

Range may span:
- first partial word
- zero or more whole words
- final partial word

Implementation modifies words using masks and tracks popcount delta.

## Set Algebra

For equal universes:

Union:

    A ∪ B -> wordwise OR

Intersection:

    A ∩ B -> wordwise AND

Difference:

    A \ B -> A & ~B

Padding bits are kept zero.

## Ordered Iteration

Bitmap iteration uses:

    next_member(start)

which scans:
- masked first word
- then whole words
- ctz on first nonzero word

Thus members are produced in ascending integer order.

## Dense vs Sparse

Dense bitmap storage:

    Theta(U / 64) words

regardless of cardinality.

Great when:
- universe bounded
- density moderate/high
- fast set algebra matters

Poor when:
- U enormous
- only a few members

Later Chapter 090+ will revisit compressed/succinct structures.
Roaring Bitmap is an important practical hybrid structure, but this chapter intentionally teaches dense bitmap fundamentals first.

## Complexity

Let W=ceil(U/64).

- contains/add/remove: O(1)
- cardinality: O(1)
- set/clear range: O(words touched)
- union/intersection/difference: O(W)
- next member: O(words scanned)
- storage: Theta(W)

## Files

- src/int_bitmap.*
- tests/test_bitmap.c
- examples/bitmap_demo.c
- benchmarks/bitmap_benchmark.c
- standard chapter artifacts
