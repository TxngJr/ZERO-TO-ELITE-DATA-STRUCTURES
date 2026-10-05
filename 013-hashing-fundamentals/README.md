# Chapter 013 — Hashing Fundamentals

## Goal

Hashing คือการแปลง key ให้เป็นค่าจำนวนเต็มที่กระจายตัวดีเพื่อนำไปเลือก bucket/index อย่างรวดเร็ว บทนี้ยังไม่สร้าง Hash Table เต็มรูปแบบ แต่สร้าง mental model และเครื่องมือวัดที่จำเป็นก่อน

หลังจบบทนี้คุณควร:
- แยก hash value, bucket index และ key equality
- เข้าใจ deterministic hash function
- เข้าใจ collision ว่าหลีกเลี่ยงไม่ได้เมื่อ key space ใหญ่กว่า bucket space
- อธิบาย load factor
- แยก hash quality ออกจาก table collision strategy
- เข้าใจ avalanche effect แบบ intuition
- รู้จัก integer hashing และ byte/string hashing
- เข้าใจ modulo vs power-of-two bucket indexing
- อธิบาย birthday-paradox intuition สำหรับ collisions
- เข้าใจ adversarial inputs และเหตุผลของ randomized/seeded hashing
- วัด bucket distribution
- เข้าใจว่าค่า hash เท่ากันไม่ได้หมายถึง key เท่ากัน

## 1. Hash Function

แนวคิด:

    key -> hash(key) -> large integer

Hash function ที่ดีสำหรับ Hash Table ทั่วไปควร deterministic ภายใต้ contract เดียวกัน, เร็ว และกระจาย input ที่คาดว่าจะเจอได้ดี

Hash function สำหรับ table ไม่จำเป็นต้องเป็น cryptographic hash.

## 2. Hash Value vs Bucket Index

ถ้ามี bucket_count=m:

    bucket = hash(key) % m

หรือถ้า m เป็น power of two:

    bucket = hash(key) & (m-1)

bit-mask indexing พึ่งพาคุณภาพ low bits มากขึ้น จึงต้องแยก hash quality ออกจาก reduction strategy.

## 3. Collision

Collision หมายถึง keys ต่างกันแต่ถูก map ไป bucket เดียวกัน

ถ้า key space ใหญ่กว่า bucket count, collision หลีกเลี่ยงไม่ได้ตาม pigeonhole principle.

Hash Table จึงต้องมี collision-resolution strategy เช่น:
- separate chaining
- open addressing
- cuckoo hashing
- perfect hashing ในกรณีพิเศษ

## 4. Equality Still Matters

Hash ช่วยหา "ตำแหน่งที่ควรค้นต่อ" แต่ไม่ได้แทน equality.

Correct lookup ต้องเปรียบเทียบ actual key เพราะ different keys can collide.

## 5. Load Factor

ให้ n=stored entries และ m=buckets/slots:

    alpha = n / m

Separate chaining สามารถ alpha>1.
Open addressing ต้องมี empty slots จึง alpha<1 และ performance แย่ลงเมื่อ table ใกล้เต็ม.

## 6. Uniform Hashing Intuition

Idealized model คือ keys กระจายค่อนข้างสม่ำเสมอ.
ถ้า n keys, m buckets:

    expected occupancy ≈ n/m = alpha

real inputs/hash functions อาจไม่ uniform จึงต้อง resize/test/measure.

## 7. FNV-1a Teaching Hash

บทนี้ใช้ FNV-1a สำหรับ byte examples เพราะ implementation สั้นและช่วยเห็น iterative mixing:

    hash = offset_basis
    for each byte:
        hash ^= byte
        hash *= prime

นี่ไม่ใช่คำแนะนำว่า FNV-1a เหมาะที่สุดสำหรับทุก production/adversarial setting.

## 8. Integer Mixing

การใช้ key % m ตรง ๆ อาจแย่เมื่อ input มี pattern สัมพันธ์กับ m.

ตัวอย่าง keys 0,16,32,48 กับ m=16 จะลง bucket 0 ทั้งหมด.

Integer mixing พยายามกระจาย bits ก่อน reduction.

## 9. Avalanche Intuition

เปลี่ยน input เล็กน้อย แล้ว output bits ควรเปลี่ยนหลายตำแหน่งอย่างไม่สัมพันธ์ตรงไปตรงมา.

Avalanche ไม่ใช่ metric เดียวของ hash quality แต่ช่วยลด pattern leakage.

## 10. Birthday-Paradox Intuition

แม้ hash space ใหญ่ collision chance เริ่มมีนัยสำคัญก่อนใช้ space เต็มมาก.

แต่ Hash Table มักลด hash ไป bucket_count ที่เล็กกว่ามาก จึงต้องออกแบบ collision resolution ตั้งแต่แรก.

## 11. Hashing Is Not Encryption

Hashing, encryption และ password hashing เป็นคนละปัญหา.

อย่านำ table hash ง่าย ๆ ไปใช้เก็บรหัสผ่าน.

## 12. Adversarial Inputs

ถ้าผู้โจมตีเลือก keys ที่ชนกันจำนวนมาก expected O(1) lookup อาจ degrade ใกล้ O(n).

runtime บางตัวใช้ random seed, keyed hashing หรือ mitigation อื่น.

## 13. Distribution Experiment

repository มีโปรแกรมวัด bucket occupancy:
- empty buckets
- min occupancy
- max occupancy

อย่าตัดสิน hash จาก sample เดียวหรือ metric เดียว.

## 14. Complexity Preview

fixed-size integer hash:
    Θ(1) ภายใต้ machine-word model

string/byte hash length L:
    Θ(L)

full Hash Table complexity จะขึ้นกับ collision strategy/load factor ใน Chapter 014.

## 15. Hash Contract

ถ้า equality บอก a==b ต้องได้ hash(a)==hash(b).

กลับกันไม่จริง: hash(a)==hash(b) ไม่ได้หมายถึง a==b.

นี่คือ contract สำคัญของ Hash Map/Set.
