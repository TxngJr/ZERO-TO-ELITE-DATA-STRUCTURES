# Theory

Non-blocking progress levels ต้องแยกจาก correctness:
- linearizability บอก object history ดูเหมือน sequential ได้หรือไม่
- lock-free บอก system progress
- wait-free บอก per-operation progress bound

CAS failure ใน lock-free algorithm มักเป็น evidence ว่ามี concurrent operation อื่นเปลี่ยน state จึงช่วย system-wide progress argument แต่ไม่ได้รับประกัน fairness ให้ thread ปัจจุบัน.

Memory reclamation เป็นส่วนหนึ่งของ correctness ไม่ใช่ cleanup หลังบ้าน. Algorithm ที่ CAS ถูกแต่ free/reuse node เร็วเกินไปยังผิดได้.
