# Chapter 069 — Counting Bloom Filter

## Goal

Counting Bloom Filter แทน bit แต่ละตำแหน่งด้วย small counter เพื่อให้ decrement เมื่อลบ key occurrence ได้.

บทนี้ใช้ uint16_t counters และ double hashing แบบ Chapter 068.

## Query Semantics

maybe_contains(key):
- ถ้ามี hashed counter ใดเป็น 0 -> definitely absent
- ถ้าทุก counter > 0 -> maybe present

ยังคงมี false positives.

## Deletion Contract

ข้อจำกัดสำคัญมาก:

> caller ควร remove เฉพาะ key occurrence ที่รู้จริงว่าเคย add และยังไม่ได้ remove หมด.

เพราะ false-positive key อาจมี counters > 0 จาก keys อื่น. ถ้าลบ key ที่ไม่เคย add จริง counters ของ keys อื่นอาจถูก decrement และสร้าง false negative.

Counting Bloom Filter จึงไม่ได้ทำให้ membership เป็น exact set.

## Counter Saturation

uint16_t max = 65535.

บทนี้ไม่ใช้ saturating increment เพราะ saturation แล้ว decrement อาจทำให้ข้อมูล contribution หาย.

ถ้า add พบ counter เต็ม:
- rollback increments ของ hash positions ก่อนหน้าใน operation เดียวกัน
- return false
- structure กลับสู่ state ก่อน add

Duplicate hash positions ใน key เดียวกันก็ถูก handle เพราะ rollback recompute positions ตามลำดับเดียวกัน.

## Transactional Remove

Remove decrement ทีละ hash position.

ถ้าพบ counter 0 ระหว่าง operation:
- rollback decrements ก่อนหน้า
- return false

อย่างไรก็ตาม rollback นี้ตรวจได้เฉพาะ underflow ไม่สามารถพิสูจน์ว่า key เป็น true member.

## Metadata

`logical_count` คือจำนวน successful add calls ลบ successful remove calls ภายใต้ caller contract.

`nonzero_count` cache จำนวน counters ที่ >0 และ validator recompute เพื่อตรวจ metadata.

## Complexity

add/remove/query = O(k). Storage O(m counters), โดยบทนี้ counter ละ 16 bits.

## Files

- src/byte_counting_bloom.*
- tests/test_counting_bloom.c
- examples/counting_bloom_demo.c
- benchmarks/counting_bloom_benchmark.c
