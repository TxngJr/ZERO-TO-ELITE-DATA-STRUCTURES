# Chapter 091 — Succinct Data Structures

## Chapter Overview

Succinct Data Structures ศึกษาวิธีเก็บข้อมูลให้ใกล้ **information-theoretic minimum** แต่ยัง query ได้โดยไม่ต้อง decompress ทั้งก้อนก่อน บทนี้ต่อจาก Chapter 090 ซึ่งเน้น compression ของ monotonic sequence: compression ทั่วไปมักยอมแลก random access กับพื้นที่ ส่วน succinct structure ตั้งโจทย์แรงกว่า — ต้องการ representation ใกล้ขอบล่างเชิงข้อมูลพร้อม operations ที่เร็ว เช่น `access`, `rank`, `select`.

แกนปฏิบัติของบทคือ **static packed bit vector** ที่รองรับ:

- `access(i)` — อ่าน bit ตำแหน่ง `i`
- `rank1(end)` — จำนวน bit 1 ในช่วง `[0,end)`
- `rank0(end)` — จำนวน bit 0 ในช่วง `[0,end)`
- `select1(k)` — index ของ bit 1 ตัวที่ `k` แบบ zero-based
- `select0(k)` — index ของ bit 0 ตัวที่ `k`
- validator สำหรับ padding และ rank directory

> ข้อสำคัญ: implementation ในบทนี้เป็น **succinct-style engineering bridge** ไม่ได้อ้างว่าเป็นโครงสร้าง `n + o(n)` bits แบบ asymptotic เต็มรูปแบบ เพราะใช้ superblock ขนาดคงที่ 512 bits และ `size_t` prefix directory. เราแยกสิ่งที่พิสูจน์เชิงทฤษฎีได้ออกจากสิ่งที่ implementation นี้รับประกันจริงอย่างชัดเจน.

## Learning Objectives

หลังจบบท ผู้เรียนต้องสามารถ:

1. แยก `compressed` ออกจาก `compact` และ `succinct` ได้
2. อธิบาย lower bound ของ bit vector ที่มี `n` bits ได้
3. นิยาม `rank` และ `select` ด้วย half-open prefix contract ได้
4. pack logical bits ลง `uint64_t[]` และจัดการ final-word padding อย่างถูกต้อง
5. สร้าง rank directory และวิเคราะห์ overhead ของ metadata ได้
6. implement `rank1/rank0/select1/select0` โดยไม่ unpack ทั้ง vector
7. อธิบาย trade-off ระหว่าง directory density, query latency และ extra bytes
8. ทดสอบ boundary ที่ 63/64/65 และ 511/512/513 bits ได้
9. อธิบายว่าทำไม practical fixed-block directory ยังไม่เท่ากับ strict `n + o(n)` succinctness
10. เชื่อม rank/select ไปยัง compressed indexes, wavelet structures และ text indexes ในบทขั้นสูงได้

## Prerequisites

- Chapter 064 Bitset
- Chapter 066 Bit Vector
- Chapter 090 Compressed Data Structures
- Chapter 004 Complexity Analysis
- Chapter 002 Memory Fundamentals

## Mental Model

ให้มอง bit vector เป็นถนนยาวของ 0/1:

```text
index:  0 1 2 3 4 5 6 7 8 9
bits :  1 0 1 1 0 0 1 0 1 1
```

`rank1(7)` ถามว่า “ก่อน index 7 มี 1 กี่ตัว?”

```text
[ 1 0 1 1 0 0 1 ) 0 1 1
  ^^^^^^^^^^^^^^^
  4 ones
```

`select1(2)` ถามว่า “1 ตัวที่ 3 อยู่ index ไหน?” เพราะ `k` เป็น zero-based:

```text
1st? zero-based k=0 -> index 0
k=1 -> index 2
k=2 -> index 3
```

ถ้าทุก query ต้อง scan จาก bit 0 จะเป็น Θ(n). แนวคิด directory คือเก็บ checkpoint เป็นช่วง ๆ:

```text
superblock 0                superblock 1
bits 0..511                 bits 512..1023
rank before = 0             rank before = 247
```

จาก checkpoint เราสแกนเฉพาะ word ภายใน superblock แทนทั้ง vector.

## Formal Model

ให้ `B[0..n)` เป็น bit vector.

```text
rank1(i) = sum(B[j]) for 0 <= j < i
rank0(i) = i - rank1(i)
```

และ:

```text
select1(k) = minimum i such that B[i] = 1 and rank1(i+1) = k+1
```

ถ้า `m` คือจำนวน ones, `select1(k)` นิยามเมื่อ `0 <= k < m`.

ในเชิง succinct theory โครงสร้างสำหรับ bit vector มักตั้งเป้า:

```text
n + o(n) bits
```

พร้อม `rank/select` ที่เร็วมากหรือ O(1) ภายใต้ word-RAM assumptions. บทนี้ใช้ representation เชิงปฏิบัติที่เข้าใจ memory layout ได้ตรง ๆ ก่อนเข้าสู่โครงสร้าง research-level ใน Chapter 168.

## Memory Representation

```text
SuccinctBitVector
├── nbits
├── word_count
├── super_count
├── ones
├── words[]       packed payload, 64 logical bits / uint64_t
└── super_rank[]  number of ones before each 512-bit superblock
```

Bit `i` อยู่ที่:

```text
word = i / 64
bit  = i % 64
mask = 1ULL << bit
```

Padding bits ใน word สุดท้ายต้องเป็นศูนย์ ไม่เช่นนั้น `select0` หรือ validator อาจนับพื้นที่ที่ไม่ใช่สมาชิก logical vector.

## Core Operations

### access

อ่านหนึ่ง word แล้ว mask หนึ่ง bit.

### rank1

1. หา full words และ remainder
2. หา superblock ที่เกี่ยวข้อง
3. เริ่มจาก checkpoint `super_rank[s]`
4. popcount words ภายใน superblock ก่อน target
5. popcount masked remainder word

เพราะ superblock มี 8 words คงที่ งานหลัง checkpoint ถูก bound ด้วยค่าคงที่.

### select1

1. binary search `super_rank` เพื่อหา superblock ที่ครอบ `k`
2. scan ไม่เกิน 8 words
3. scan set bits ใน word เป้าหมายจนถึงลำดับที่ต้องการ

### select0

เหมือน `select1` แต่ complement word แล้ว mask เฉพาะ logical bits เพื่อไม่ให้ padding กลายเป็น zero candidates ปลอม.

## Correctness Sketch

Invariant สำคัญ:

- `super_rank[s]` เท่ากับจำนวน ones ก่อน superblock `s`
- `super_rank` ไม่ลดลง
- padding bits เป็นศูนย์
- `ones == super_rank[super_count]`

ดังนั้น `rank1` เริ่มจาก prefix ที่ถูกต้อง แล้วบวกเฉพาะ bits ที่เหลือก่อน `end`; ไม่มี bit ถูกนับซ้ำหรือข้าม. `select1` binary-search checkpoint ที่ยังไม่เกินลำดับเป้าหมาย แล้ว local scan ตามลำดับ index จึงคืน occurrence ที่ถูกต้อง.

## Real-World Connections

Rank/select เป็น primitive สำคัญใน:

- succinct trees
- wavelet trees / wavelet matrices
- FM-index และ compressed text indexes
- bitmap indexes
- compressed graph representations
- static set encodings
- columnar analytics

## When NOT to Use This Structure

ไม่เหมาะเมื่อ:

- bits เปลี่ยนบ่อยและต้อง insert/delete ตรงกลาง
- metadata overhead สำคัญมากกว่าความเร็ว query
- workload มีเพียง sequential scan
- ต้องการ strict theoretical succinct bound แต่ implementation ยังใช้ fixed block metadata
- vector เล็กมากจน plain integer/array ง่ายกว่า

## Build → Run → Observe → Explain

```bash
cmake -S . -B build
cmake --build build --target ch091_succinct_tests ch091_succinct_benchmark
ctest --test-dir build -R ch091 --output-on-failure
./build/ch091_succinct_benchmark
```

ให้สังเกต `packed_payload` เทียบกับ `total_structure`: payload อาจเล็ก แต่ directory และ object metadata ยังเป็น memory จริงที่ต้องนับเสมอ.
