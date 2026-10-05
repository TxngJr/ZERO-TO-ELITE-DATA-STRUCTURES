# Chapter 064 — Bitset

## Goal

Bitset เก็บ boolean จำนวนมากด้วย 1 bit ต่อค่าแทนการใช้ byte/int ต่อค่า.

บทนี้ implement fixed-length mutable Bitset บน:

    uint64_t words[]

รองรับ:
- set / clear / flip / test
- set_all / clear_all / flip_all
- count / any / all / none
- AND / OR / XOR / NOT
- find_next_set
- structural validation

## Packed Representation

Bit index i:

    word = i / 64
    bit  = i % 64

Mask:

    1ULL << bit

ดังนั้น 1 word เก็บ 64 flags.

Memory โดยประมาณ:

    ceil(n / 64) * 8 bytes

เทียบกับ byte-per-boolean:

    n bytes

## Padding Bits

ถ้า bit_count ไม่หาร 64 ลงตัว เช่น 70 bits:

- words[0] ใช้ 64 bits
- words[1] ใช้จริง 6 bits
- อีก 58 bits เป็น padding

Invariant:

> padding bits ใน word สุดท้ายต้องเป็น 0 เสมอ

จึงต้อง mask หลัง:
- set_all
- flip_all
- NOT
- bitwise ops ที่อาจเผย garbage padding

## Word-Level Operations

AND/OR/XOR ทำทีละ 64 bits:

    out_word = a_word OP b_word

จึงประมวลผล 64 flags ต่อหนึ่ง machine-word operation.

Complexity:
- single-bit op: O(1)
- count: O(number of words)
- bitwise combine: O(number of words)
- find_next_set: O(words scanned)

## Popcount

ใช้ compiler builtin เมื่อ GCC/Clang:

    __builtin_popcountll

แนวคิดคือ count set bits ต่อ word.

ในบทหลังจะเชื่อม popcount ไปยัง rank/select ของ Bit Vector.

## find_next_set

ค้น bit ที่เป็น 1 ตัวแรกตั้งแต่ index start.

Algorithm:
1. mask bits ก่อน start ใน word แรก
2. ถ้า word nonzero หา least-significant set bit
3. ถ้าไม่มี scan word ถัดไป

ใช้:

    __builtin_ctzll

เฉพาะเมื่อ word != 0 เท่านั้น เพราะ ctz(0) ไม่ควรถูกเรียก.

## Bitset vs Bitmap vs Bit Vector

Bitset:
- abstraction เป็น finite set of boolean positions
- focus operations/boolean algebra

Bitmap:
- มักใช้ represent membership ของ integer universe หรือ occupancy/image-like map
- focus set membership/cardinality/iteration

Bit Vector:
- sequence of bits
- มักมี rank/select/access indexing metadata
- Chapter 066

## Files

- src/int_bitset.*
- tests/test_bitset.c
- examples/bitset_demo.c
- benchmarks/bitset_benchmark.c
- standard learning artifacts
