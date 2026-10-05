# Chapter 071 — Cuckoo Hashing

Cuckoo Hashing เก็บ exact keys ในสอง hash tables. Key k ถูกต้องได้ที่ h1(k) ใน table1 หรือ h2(k) ใน table2.

Lookup ตรวจเพียงสองตำแหน่ง จึง O(1) worst-case หลัง table ถูกสร้าง.

Insertion อาจ kick key ออกจากตำแหน่งหนึ่งไปยัง alternate table. ถ้า kick cycle เกิน limit implementation rollback snapshot แล้ว rebuild ด้วย capacity ใหญ่ขึ้นและ seeds ใหม่.

Set semantics: duplicate insert ไม่เพิ่ม size. ไม่มี false positives เพราะเก็บ full uint64_t key.

Load policy ในบทนี้ resize เมื่อ size+1 > capacity แม้มีสอง tables รวม 2*capacity slots เพื่อรักษา conservative load และลด cycles.

Expected insertion O(1) amortized under good hashing; rebuild worst-case O(n). Lookup/remove O(1).
