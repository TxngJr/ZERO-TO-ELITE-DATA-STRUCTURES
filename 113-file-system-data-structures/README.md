# Chapter 113 — File-System Data Structures

บทนี้สร้าง **mini filesystem metadata model** เพื่อศึกษาความสัมพันธ์ของ:
- inode table
- block bitmap
- directory entries
- direct block pointers
- single-indirect block
- file-size → logical-block mapping

Implementation เป็น in-memory metadata simulator ไม่ใช่ filesystem driver.

## Inode Layout

แต่ละ file inode รองรับ:
- 4 direct data blocks
- 1 single-indirect metadata block
- 64 indirect data-block entries

ดังนั้น file หนึ่งมี data blocks สูงสุด 68 blocks.

```text
inode
├─ direct[0..3] ──> data blocks
└─ indirect_block ──> table
                      ├─ indirect[0]
                      ├─ indirect[1]
                      └─ ... 64 entries
```

Indirect metadata block ถูกจองจาก block bitmap เดียวกับ data blocks เพื่อให้เห็น metadata overhead จริง.

## Directories

ทุก non-root inode มี directory entry หนึ่งรายการ:

```text
(parent inode, name) -> child inode
```

Model นี้ไม่มี hard links จึงทำ ownership validation ได้ชัด.

## Growth Safety

ก่อนขยาย file:
1. คำนวณจำนวน data blocks เพิ่ม
2. คำนวณว่าต้องสร้าง indirect metadata block หรือไม่
3. preselect free physical blocks ทั้งหมด
4. commit bitmap + inode mapping หลังได้ครบ

จึงไม่ทิ้ง partial allocation เมื่อ free blocks ไม่พอ.

## Tests

- root directory
- 10 subdirectories
- 1,000 files
- randomized file sizes crossing direct/indirect boundary
- directory lookup verification
- block mapping checks
- shrink-to-zero on half the files
- full bitmap/inode/directory validator
- ASan/UBSan
