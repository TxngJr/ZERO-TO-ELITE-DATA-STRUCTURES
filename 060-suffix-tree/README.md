# Chapter 060 — Suffix Tree

## Goal

Suffix Tree เป็น compressed trie ของ suffix ทุกตัวใน text.

บทนี้ implement **explicit compressed suffix tree** สำหรับ arbitrary bytes โดยเพิ่ม unique sentinel symbol:

    256

ซึ่งอยู่นอก byte alphabet 0..255.

รองรับ:
- substring contains
- occurrence count
- occurrence-position report
- structural validation

## Why a Unique Sentinel?

ถ้าไม่มี sentinel suffix หนึ่งอาจเป็น prefix ของ suffix อื่น.

ตัวอย่าง:

    "aaa"

suffix "a" จบกลาง path ของ suffix "aa".

เมื่อเติม sentinel ที่ไม่ปรากฏในข้อมูล:

    "aaa$"

ทุก suffix แตกต่างกันจนถึงปลายและจบที่ leaf.

Implementation ไม่ใช้ byte '$' จริง.
ใช้ symbol integer 256 จึงไม่ชนกับ input byte ใด.

## Compressed Edges

Edge ไม่ copy substring.

เก็บเพียง:

    start
    end

เป็น half-open interval ใน owned symbol array:

    symbols[start..end)

ดังนั้น edge label memory เป็น O(1).

Internal nodes เกิดเมื่อ insertion พบ mismatch กลาง edge แล้ว split edge.

## Teaching Construction

สร้าง suffixes:

    0,1,2,...,n

รวม suffix sentinel-only ที่ n.

แต่ละ suffix ถูก insert จาก root โดยเปรียบเทียบ compressed edge labels.

เวลา worst case:

    O(n²)

ตัวอย่าง repeated text เช่น:

    aaaaa...

ทำให้ suffix insertion เปรียบเทียบ path ยาวมากซ้ำ ๆ.

นี่ตั้งใจเพื่อให้เห็น representation/invariants ชัดก่อน.

Production-famous algorithm:

    Ukkonen

สร้าง suffix tree แบบ online ใน O(n) สำหรับ fixed alphabet/model assumptions ด้วย:
- suffix links
- active point
- implicit states
- shared leaf ends

ซับซ้อนกว่ามากและควรเรียนหลังเข้าใจ structure ก่อน.

## Search

Pattern เป็น bytes และต้องไม่ว่าง.

Start at root:
1. choose outgoing edge by first byte
2. compare pattern against edge label
3. if pattern endsกลาง edge -> match สำเร็จ
4. if edge ends first -> continue at child
5. mismatch -> absent

If pattern locus is:
- at node, or
- inside incoming edge to node

all leaves below that child correspond to occurrences.

## Occurrence Count

Every original-text occurrence corresponds to one suffix whose prefix is pattern.

Node caches:

    leaf_count

So after locating pattern locus:

    count = leaf_count(locus child)

Search cost:
    O(m * child-lookup factor)

With byte alphabet child degree <=257.
This implementation uses linear child lookup for teaching clarity.

## Occurrence Report

Collect:

    suffix_start

from descendant leaves.

Sentinel-only suffix start n cannot match a nonempty byte pattern.

Report order follows tree traversal, not numerical text position.

## Structural Size

Compressed suffix tree has O(n) nodes/edges.

With unique sentinel:
- n+1 suffix leaves
- internal branching nodes
- at most O(n) total nodes

The implementation preallocates a safe node arena proportional to symbol count.

## Complexity

For this teaching implementation:

| Operation | Complexity |
|---|---:|
| build | O(n²) worst case |
| contains | O(m·d) with linear child scan |
| count | O(m·d) after leaf_count metadata |
| report | O(m·d + k) |
| storage | O(n) |

d <= 257 outgoing edges for byte alphabet + sentinel.

## Suffix Tree vs Suffix Array

Suffix Array:
- compact arrays
- binary-search patterns
- simpler memory layout
- Chapter 059 build O(n log² n) here

Suffix Tree:
- explicit compressed branching
- pattern traversal follows characters
- richer structural basis
- pointer/index-heavy representation

## Files

- src/byte_suffix_tree.*
- tests/test_suffix_tree.c
- examples/suffix_tree_demo.c
- benchmarks/suffix_tree_benchmark.c
- standard learning artifacts
