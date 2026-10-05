# Chapter 030 — Radix Tree / Patricia Trie

## Goal

Ordinary Trie ใช้หนึ่ง edge ต่อหนึ่ง symbol. ถ้ามี long unary path จะเกิด nodes จำนวนมากที่ไม่มี branching.

Radix Tree หรือ Patricia-style compressed trie รวม unary path ให้เป็น edge label หลาย bytes.

บทนี้สร้าง Byte Radix Tree ที่:
- edge label เป็น owned byte slice
- split edge เมื่อ key แตกกลาง label
- exact lookup
- prefix lookup แม้ prefix จบกลาง edge
- prefix count
- deletion + path compression
- lexicographic traversal

## Trie vs Radix Tree

Trie:

    c - a - r - p - e - t

Radix:

    "carpet"

หนึ่ง edge สามารถเก็บหลาย bytes.

## Edge Invariant

ทุก edge มี:
- label pointer
- label length > 0
- child

Outgoing edges จาก node เดียวกันต้องมี first byte ต่างกัน.

## Insert — No Matching Edge

ถ้าไม่มี edge first byte ตรง:
- create terminal leaf
- create edge label = remaining key suffix

## Insert — Full Edge Match

ถ้า key suffix เริ่มด้วย edge label ทั้งก้อน:
- consume label
- descend child
- continue

## Insert — Split Edge

Existing:

    "carpet"

Insert:

    "carbon"

Common prefix:

    "car"

Split:

             "car"
               |
            middle
            /    \
        "pet"   "bon"
          |       |
       old child new leaf

Original key path survives และ key ใหม่ได้ branch ของตัวเอง.

## Key Ends at Split

Insert "car" into existing "carpet":

middle node itself becomes terminal.
ห้ามสร้าง zero-length outgoing edge.

## Prefix Ending Inside Edge

Stored:

    "application"

Query prefix:

    "app"

prefix สามารถจบกลาง compressed edge ได้.

has-prefix ต้องคืน true ถ้า query bytes ถูก consume หมดแม้ edge label ยังเหลือ.

## Exact Search

Exact key ต้อง consume complete edge labels และจบตรง node boundary ที่ terminal.

จบกลาง edge ไม่ใช่ exact stored key.

## Delete and Recompression

หลัง clear terminal:
- remove empty child edge
- ถ้า child เป็น non-terminal และมี edge เดียว ให้ merge labels

Example:

    "car" -> node -> "pet"

becomes:

    "carpet"

## OOM During Optional Recompression

Semantic correctness ของ deletion ไม่ขึ้นกับ maximal compression.

ถ้า allocation ตอน merge label ล้ม:
- key deletion ยังถูกต้อง
- structure ยัง valid แต่ไม่ maximally compressed

Validator จึงตรวจ semantic/structural invariants ไม่บังคับ maximal compression.

## Lexicographic Traversal

Sibling edges sorted by first unsigned byte.
เพราะ first bytes unique จึงกำหนด sibling subtree order ได้.

DFS:
1. emit current terminal
2. visit edges ascending
3. append whole edge label

## Patricia Terminology

คำว่า Patricia Trie / Radix Tree / Compact Trie มีหลาย representation variants.

ต้องดู contract จริง:
- bit / byte / character radix
- edge-label scheme
- termination semantics
- compression rules

## Routing Connection

Radix/Patricia structures สำคัญกับ:
- IP routing
- longest-prefix matching
- autocomplete
- key indexes

Bitwise Patricia variants ใช้มากใน routing contexts.

## Complexity

Let L = key bytes.

Total edge-label bytes compared along one path is O(L).
Sibling edge scan bounded by byte alphabet 256.

For fixed byte alphabet:

    insert/search/prefix = O(L)

count-prefix/visit เพิ่ม matching-subtree/output cost.

## Trie vs Radix

Radix:
- fewer nodes on unary paths
- fewer pointer hops
- more complex split/merge
- label copy/allocation overhead

Trie:
- simpler one-symbol edges
- potentially more nodes

No universal winner.
