# Chapter 007 — Strings

## Goal

String คือ sequence ของ characters แต่ representation และ semantics ต่างกันมากในแต่ละภาษา บทนี้เริ่มจาก C character arrays เพื่อให้เห็น memory จริง แล้วขยายไป mutable/immutable strings, length, capacity, encoding และ dynamic string implementation

หลังจบบทนี้คุณควร:
- แยก character array กับ string abstraction
- อธิบาย null-terminated C string
- แยก logical length, allocated capacity และ byte count
- เข้าใจ mutable vs immutable string
- รู้ว่าคำว่า character อาจไม่เท่ากับ 1 byte ใน Unicode
- implement dynamic byte string จากศูนย์
- วิเคราะห์ append/insert/erase/search
- เข้าใจ why strlen is Θ(n) for ordinary C strings
- อธิบาย small-string optimization เป็น implementation concept โดยไม่สมมติว่า library ทุกตัวใช้
- เข้าใจ string interning/copying แบบ overview
- เขียน tests สำหรับ terminator, growth, overlap และ boundary cases

## 1. Character Array vs String

C array:

    char data[5] = {'h','e','l','l','o'};

นี่คือ 5 bytes/char objects แต่ยังไม่ใช่ C string เพราะไม่มี null terminator

C string ต้องมี '\0':

    char text[] = "hello";

representation:

    ['h']['e']['l']['l']['o']['\0']

logical length = 5
storage used = 6 char objects

## 2. Why null termination matters

Functions แบบ strlen เดินจาก pointer จนเจอ '\0'

ดังนั้น:

    strlen(text)

ต้อง inspect bytes ไปเรื่อย ๆ จน terminator → Θ(n)

ถ้าไม่มี terminator ภายใน accessible object แล้ว function ยังเดินต่อ จะเกิด undefined behavior

## 3. Length-aware strings

อีก design หนึ่งเก็บ length แยก:

    struct {
        char *data;
        size_t length;
        size_t capacity;
    };

ข้อดี:
- length query Θ(1)
- สามารถเก็บ '\0' ภายใน logical bytes ได้ถ้า abstraction อนุญาต
- validation ชัดเจนขึ้น

ข้อเสีย:
- metadata เพิ่ม
- API ต้องรักษา invariant ระหว่าง data/length/capacity

implementation ในบทนี้ยังคง maintain trailing '\0' เพื่อ interoperability กับ C APIs

## 4. Mutable vs Immutable

Mutable:
- content เปลี่ยนใน object เดิม
- ต้องจัดการ capacity/growth/invalidation

Immutable:
- logical value ไม่เปลี่ยนหลังสร้าง
- append อาจสร้าง object ใหม่
- reasoning/concurrency ง่ายขึ้นบางกรณี
- allocations/copies อาจเพิ่ม ถ้าไม่มี sharing/rope/persistent structure

Python str และ Java String เป็น immutable abstractions
C char buffer เป็น mutable storage
C++ std::string เป็น mutable container abstraction

## 5. Byte vs Character vs Code Point

UTF-8:
หนึ่ง Unicode code point ใช้ 1–4 bytes

ดังนั้น:
- byte length ไม่เท่ากับ user-perceived character count
- index byte ที่ k อาจอยู่กลาง encoding sequence
- reversing bytes ไม่เท่ากับ reversing Unicode characters

บทนี้สร้าง ByteString เพื่อเน้น data-structure mechanics
Unicode text algorithms ต้องมี encoding-aware layer

## 6. Dynamic string representation

ByteString:

    +----------------------+
    | data ----------------+----> ['c']['a']['t']['\0'][...]
    | length = 3           |
    | capacity = 8         |
    +----------------------+

Invariant:
- length < = capacity
- allocated storage มีอย่างน้อย capacity+1 bytes
- data[length] == '\0'
- logical bytes อยู่ใน [0,length)

## 7. Append

append one byte:
- ensure capacity length+1
- data[length] = value
- length++
- data[length] = '\0'

geometric growth ทำให้ append amortized Θ(1)

append many bytes length m:
- ensure capacity n+m
- copy bytes
- update length
- restore terminator

## 8. Insert / Erase

Insert ที่ index i:
- shift suffix rightด้วย memmove
- copy inserted bytes
- update length

time Θ(n-i + m) โดยประมาณใน RAM model

Erase count k:
- move suffix left
- reduce length
- restore terminator

## 9. Search

Naive substring search:
- worst case Θ(nm)
- n = text length
- m = pattern length

บทนี้จะไม่ implement advanced algorithms เพราะ suffix structures/Aho-Corasick อยู่ later chapters

## 10. Concatenation trap

ถ้า immutable string concatenate ทีละชิ้นแบบสร้างใหม่ทุกครั้ง:

    s = s + piece

อาจ copy prefix เดิมซ้ำจำนวนมากและเกิด quadratic behavior

แนวทางแก้:
- builder/dynamic buffer
- reserve
- rope/piece table สำหรับ workload เฉพาะ

จะกลับมาใน Chapters 084–085

## 11. Comparison

Lexicographic compare:
- compare byte/character sequence จากซ้ายไปขวา
- worst Θ(min(n,m)) จนเจอความต่างหรือจบ sequence

encoding/collation locale-aware comparison ซับซ้อนกว่านี้มาก

## 12. Common production concerns

- embedded null bytes
- encoding validity
- locale/collation
- normalization
- ownership
- substring copies vs views
- lifetime of string views
- injection/security when strings represent commands/query/code

บทนี้โฟกัส representation/data-structure ไม่ใช่ text security

## Complexity summary

| Operation | Time |
|---|---:|
| length stored in metadata | Θ(1) |
| strlen C string | Θ(n) |
| index byte | Θ(1) |
| append one amortized | Θ(1) |
| append m bytes | Θ(m) amortized excluding rare reallocation copies spread across sequence |
| insert at i | Θ(n-i+m) |
| erase at i | Θ(n-i) |
| naive find substring | O(nm) worst |

## Files

- src/byte_string.h/.c
- examples/c_string_layout.c
- tests/test_byte_string.c
- benchmarks/string_building.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
