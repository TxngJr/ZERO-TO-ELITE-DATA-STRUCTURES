# Chapter 127 — Locality of Reference

บทนี้แยก locality ออกจากคำว่า "เร็ว" แล้ววัดจาก **access trace** โดยตรง.

Locality มี 2 แกน:

- **Spatial locality** — accesses ใกล้กันอยู่ใน cache line เดียวกันหรือ line ใกล้กัน
- **Temporal locality** — line ที่เคยแตะถูกแตะซ้ำอีก

Implementation รับ trace ของ logical element indices แล้ว map เป็น byte address:

```text
byte_address = index * element_size
line_id      = byte_address / line_size
```

จากนั้นคำนวณ:

- access count
- unique cache lines
- first line touches
- temporal line reuses
- consecutive accessesใน line เดียวกัน
- line transitions
- adjacent-line transitions
- maximum referenced element index

## Deterministic Examples

128 elements, element size 8, line size 64:

Sequential 0..127:

```text
unique lines     = 16
same-line pairs  = 112
line transitions = 15
```

Stride 8 elements:

```text
0,8,16,...
```

ทุก accessขึ้น cache line ใหม่ จึงไม่มี same-line adjacency.

## Important

บทนี้ไม่ assert ว่า sequential "ต้องเร็วกว่า" random บนทุกเครื่อง.
Tests ตรวจ **trace geometry** เท่านั้น.
Timing benchmarkมีไว้สังเกต ไม่ใช่ correctness oracle.
