# Theory — Blockchain Data Structures

Blockchain เชื่อม authenticated batches ด้วย hash pointer.

Transaction Merkle root commits transactions ภายใน block.
Previous-block hash commits ordering/history ระหว่าง blocks.

ถ้า trusted tip hash เปลี่ยนไม่ได้ การแก้ transaction เก่าจะเปลี่ยน:
1. transaction leaf hash
2. Merkle root
3. block hash
4. every descendant block's previous-hash dependency

อย่างไรก็ตาม hash-linked structure ไม่ได้สร้าง consensus ด้วยตัวเอง. Consensus protocol ต้องกำหนดว่า chain/tip ใดถูกยอมรับ.
