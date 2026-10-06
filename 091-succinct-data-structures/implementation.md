# Implementation Notes

ไฟล์หลัก: `src/succinct_bit_vector.c`.

Design choices:

- C17
- input bytes ต้องเป็น 0 หรือ 1 เท่านั้น
- payload pack เป็น `uint64_t`
- 8 words ต่อ superblock
- `rank` contract ใช้ half-open `[0,end)`
- `select` ใช้ zero-based occurrence
- final padding ถูก mask เป็น zero
- `storage_bytes` นับ object + payload + directory

Implementation ใช้ portable bit counting loop แทนการผูกกับ compiler intrinsic เพื่อให้ semantics อ่านง่าย. Production build อาจเลือก `__builtin_popcountll`, hardware POPCNT หรือ vectorized kernels หลัง benchmark.

`select1/select0` ใช้ binary search บน superblock prefix และ local scan. นี่ทำให้ code เหมาะกับการเรียน correctness ก่อน optimization เช่น broadword/select tables.
