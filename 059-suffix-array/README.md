# Chapter 059 — Suffix Array

## Goal

Suffix Array เก็บตำแหน่งเริ่มต้นของ suffix ทุกตัวในลำดับ lexicographic.

สำหรับ text ยาว n:

    SA[0..n)

เป็น permutation ของ:

    0,1,2,...,n-1

และ:

    text[SA[0]..] <
    text[SA[1]..] <
    ...

บทนี้รองรับ arbitrary bytes 0..255 และสร้าง:

- suffix array
- inverse rank
- LCP array ด้วย Kasai
- substring contains/count/report

## Prefix-Doubling Build

เริ่ม rank จาก byte เดี่ยว.

ในรอบที่ span = 1,2,4,8,...

suffix i ถูกแทนด้วยคู่ rank:

    (rank[i],
     rank[i+span])

คู่ second rank ที่เลยปลาย text ใช้ sentinel rank ต่ำสุด.

Sort ทุก suffix ตาม rank pair แล้ว assign rank ใหม่.

เมื่อทุก suffix มี rank ต่างกัน:
    construction จบ.

## Complexity Honesty

Teaching implementation ใช้ qsort ทุก doubling round.

มี O(log n) rounds.
แต่ละรอบ sort n entries:

    O(n log n)

ดังนั้นรวม:

    O(n log² n)

มีอัลกอริทึมที่ดีกว่า:
- counting/radix sort ใน doubling -> O(n log n)
- SA-IS / induced sorting -> O(n)
- DC3 / skew -> linear-time families

บทนี้เลือก implementation ที่ตรวจง่ายก่อน.

## LCP Array

LCP:

    LCP[r]

คือ longest common prefix ระหว่าง:

    suffix SA[r-1]
    suffix SA[r]

โดยกำหนด:

    LCP[0] = 0

Kasai algorithm ใช้ inverse rank และ reuse ค่า h:

    O(n)

## Pattern Search

Suffixes sorted lexicographically.

For pattern P:
- lower_bound: suffix < P
- upper_bound: suffix > P, treating "P is prefix of suffix" as equal

ช่วง SA:

    [first,last)

คือ suffixes ทั้งหมดที่เริ่มด้วย P.

Therefore:
- contains -> first < last
- occurrence count -> last-first
- report -> SA positions in suffix-array order

## Binary Data

API takes:

    const uint8_t *text, size_t length

ไม่พึ่ง null terminator.

ดังนั้น byte 0 เป็นข้อมูลได้.

Pattern length ต้อง > 0 ใน search APIs.

## Complexity

| Operation | Complexity |
|---|---:|
| build SA | O(n log² n) |
| build LCP | O(n) after SA |
| SA/rank/LCP lookup | O(1) |
| substring search | O(m log n) |
| count | O(m log n) |
| report | O(m log n + k) |
| storage | Theta(n) |

m = pattern length, k = occurrences.

## Files

- src/byte_suffix_array.*
- tests/test_suffix_array.c
- examples/suffix_array_demo.c
- benchmarks/suffix_array_benchmark.c
- standard learning artifacts
