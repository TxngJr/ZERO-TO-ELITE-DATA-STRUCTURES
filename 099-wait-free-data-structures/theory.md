# Theory

Wait-free progress ต้องให้ finite bound ต่อ operation ของทุก participating thread ตาม model. การไม่มี lock อย่างเดียวไม่พอ; CAS loop ที่อาจ retry ไม่จำกัดยังเป็นเพียง lock-free ได้.

SPSC ช่วยให้ producer และ consumer เป็น single writers ของ tail/head ตามลำดับ. Cross-thread visibility ใช้ release/acquire publication.

Wait-free ไม่ได้แปลว่า operation ต้อง succeed: bounded queue สามารถตอบ full/empty ใน bounded steps ได้ และนั่นยังเป็น completed operation ตาม API contract.
