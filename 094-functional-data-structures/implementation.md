# Implementation

`FunctionalQueue` เป็น value object ที่ถือ immutable node pointers + lengths.

สำคัญ: operations ไม่เรียก full `fq_validate` เพราะ validator เดินทั้ง list และจะทำให้ fast path มี hidden Θ(n). Operations ใช้ shallow metadata checks; tests/audits เรียก full validator แยก.

Normalization เก็บ arena mark ก่อน allocate reverse-copy. หาก allocation fail จะ rollback unpublished nodes และไม่ publish half-normalized queue.
