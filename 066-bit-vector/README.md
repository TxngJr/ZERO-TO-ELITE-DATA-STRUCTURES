# Chapter 066 — Bit Vector

## Goal

Bit Vector คือ sequence ของ bits ที่ให้ operations เชิงตำแหน่ง.

บทนี้สร้าง **immutable packed bit vector with rank/select directory**.

รองรับ:
- access(i)
- rank1(end)
- rank0(end)
- select1(k)
- select0(k)
- total ones/zeros
- structural validation

Input เป็น byte array ที่แต่ละ element ต้องเป็น 0 หรือ 1.

## Rank

Definition:

    rank1(end)

= จำนวน 1 bits ใน half-open prefix:

    [0, end)

ดังนั้น:

    rank1(0) = 0
    rank1(n) = total ones

และ:

    rank0(end) = end - rank1(end)

## Select

Definition:

    select1(k)

= index ของ one-bit ลำดับที่ k แบบ 0-based.

Example bits:

    0 1 0 1 1 0

one positions:

    [1,3,4]

Therefore:

    select1(0) = 1
    select1(1) = 3
    select1(2) = 4

Similarly select0(k) selects kth zero.

## Representation

Bits packed in:

    uint64_t words[]

Rank directory:

    prefix_ones[w]

stores number of 1 bits before word w.

Directory length:

    word_count + 1

Invariant:

    prefix_ones[0] = 0

    prefix_ones[w+1]
      = prefix_ones[w]
        + popcount(words[w])

## O(1) Rank

For end position p:

    word = p / 64
    offset = p % 64

Rank equals:
- ones before whole word
- plus popcount of first offset bits

When p == n and n is word-aligned, word may equal word_count; directory handles this directly.

## Select by Prefix Directory

To select kth one:

Need word w satisfying:

    prefix_ones[w] <= k
    prefix_ones[w+1] > k

Find w by binary search over words:

    O(log W)

Then find local kth set bit inside one uint64_t.

This implementation clears least-significant set bits repeatedly:

    word &= word - 1

until target local rank, then uses ctz.

Because one word has at most 64 bits, local work is bounded by 64.

## select0

Zero prefix before word w:

    bits_before_word - prefix_ones[w]

where:

    bits_before_word = min(w*64, n)

Binary-search a word containing kth zero.

Final word masks padding so padding zeros are never selectable.

## Why Immutable?

Rank directory is derived metadata.

If arbitrary bits mutate:
- all later prefix counts might become stale

For this chapter:
- build once
- query many times

Dynamic rank/select can use:
- Fenwick Tree
- Segment Tree
- balanced bit sequence
- succinct dynamic structures

later in the course.

## Complexity

Let W=ceil(n/64).

Build:
    O(n + W)

Access:
    O(1)

rank1/rank0:
    O(1)

select1/select0:
    O(log W + 64)
    = O(log W)

Storage:
    packed bits O(W)
    prefix directory O(W)

This directory is not yet succinct:
metadata can use comparable or greater bytes than packed bits.

Chapter 091 Succinct Data Structures will revisit lower-overhead rank/select indexes.

## Files

- src/int_bit_vector.*
- tests/test_bit_vector.c
- examples/bit_vector_demo.c
- benchmarks/bit_vector_benchmark.c
- standard learning artifacts
