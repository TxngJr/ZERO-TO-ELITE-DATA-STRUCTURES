# Chapter 070 — Cuckoo Filter

Cuckoo Filter เก็บ short fingerprints แทน full keys ใน buckets สองตำแหน่งที่สลับกันได้.

Implementation นี้ใช้ 16-bit fingerprint, 4 slots/bucket, bucket count เป็น power of two และ relocation สูงสุด 500 kicks.

Candidate buckets:
    i1 = hash(key) & mask
    i2 = i1 XOR hash(fingerprint) & mask

Alternative-index relation เป็น involution: จาก bucket ใดและ fingerprint เดิมสามารถกลับไปอีก bucket ได้.

Insert จะลอง free slot ก่อน หากเต็มจะ kick fingerprint ไป alternate bucket. เพื่อ correctness เมื่อ relocation ล้มเหลว implementation snapshot table ก่อน kick และ restore ทั้ง table + RNG state เมื่อเกิน kick limit.

Query positive = maybe present เพราะ fingerprint collision ยังเกิดได้. Deletion ควรใช้กับ key ที่ caller เชื่อว่าเคย insert; การลบ false positive อาจลบ fingerprint ของ key อื่น.

Complexity expected O(1), relocation bounded by MAX_KICKS; worst-case insertion can fail when table/load/cycle ไม่เหมาะ.

Files: src, tests, examples, benchmarks พร้อม standard chapter artifacts.
