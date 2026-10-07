# Chapter 126 — Memory Alignment & Padding

บทนี้ทำให้ alignment/padding เป็น data-structure problem ที่วัดและตรวจได้จริง.

Implementation มี 2 ส่วน:

1. **Natural Layout Calculator**
2. **Aligned Strided Array**

## Natural Layout

กำหนดแต่ละ field ด้วย:

- size
- required alignment

Algorithm:

1. align current offset ขึ้นตาม alignment ของ field
2. วาง field
3. track maximum alignment
4. ปิดท้าย struct sizeให้เป็น multiple ของ maximum alignment

ตัวอย่าง:

```text
u8  align1
u64 align8
u32 align4
```

ได้:

```text
offset 0  : u8
offset 1-7: padding
offset 8  : u64
offset 16 : u32
offset 20-23: tail padding
sizeof = 24
```

Reorderเป็น `u64,u32,u8` ได้ size 16.

## Aligned Strided Array

ถ้า element logical size 24 แต่ต้องการทุก elementเริ่มที่ 64-byte boundary:

```text
stride = align_up(24,64) = 64
```

allocator over-allocates, aligns base pointer manually และเก็บ raw pointerสำหรับ free.

## Tests

- known natural-layout offsets
- field reordering 24 → 16 bytes
- exact padding counts
- invalid non-power-of-two alignment rejection
- overflow-safe align/add/multiply failures
- 10,000 elements at 64-byte alignment
- stride 24→64 and 80→128
- every element address alignment verification
- ASan/UBSan

## Scope

บทนี้อธิบาย storage layout; ไม่อ้างว่า padding=cache performanceดีเสมอ. Over-alignmentเพิ่ม memory footprintและอาจทำ cache density แย่ลง.
