# Chapter 068 — Bloom Filter

## Goal

Bloom Filter เป็น probabilistic membership structure ที่ใช้ packed bit array + หลาย hash positions.

Semantics สำคัญ:
- negative -> definitely absent
- positive -> maybe present

ภายใต้ insert-only operation และ implementation ถูกต้อง จะไม่มี false negative สำหรับ keys ที่เพิ่มแล้ว แต่สามารถมี false positive ได้.

## Double Hashing

บทนี้สร้าง hash หลักสองค่า h1/h2 แล้ว derive k positions:

    index_i = (h1 + i*h2) mod m

h2 ถูกบังคับให้เป็น odd/nonzero-like step เพื่อหลีกเลี่ยง degenerate zero step.

## Parameters

- m = bit_count
- k = hash_count
- n = insertion calls

Approximate false-positive behavior ตามโมเดลอิสระ:

    p ≈ (1 - exp(-k*n/m))^k

สูตรเป็นโมเดล ไม่ใช่ correctness guarantee.

## Duplicate Inserts

Bloom Filter ไม่รู้ว่า key distinct หรือไม่. Insert key เดิมซ้ำเพียง set bits เดิมอีกครั้ง แต่ insertion counter ในบทนี้นับ API calls.

## Empty Key

Empty byte sequence เป็น key ที่ถูกต้องและ hash ได้. NULL key pointer อนุญาตเฉพาะเมื่อ length == 0.

## Complexity

Add/maybe_contains ใช้ O(k). Storage O(m) bits.

## Files

- src/byte_bloom_filter.*
- tests/test_bloom_filter.c
- examples/bloom_filter_demo.c
- benchmarks/bloom_filter_benchmark.c
