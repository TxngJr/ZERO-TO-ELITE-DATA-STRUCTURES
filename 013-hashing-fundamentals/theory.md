# Theory — Hashing Fundamentals

## Domain and Codomain

Hash function map domain ใหญ่ไป finite output range.

arbitrary strings มีจำนวนค่ามากกว่า 64-bit outputs มาก ดังนั้น collision ต้องมีในเชิงคณิตศาสตร์.

## Reduction Bias

hash % m อาจมี modulo bias เล็กน้อยถ้า output range ไม่หารด้วย m ลงตัว.

สำหรับ ordinary hash tables มักสำคัญน้อยกว่าคุณภาพ hash และ workload pattern แต่ควรแยกจาก random/security contexts.

## Seeded Hashing

family:

    h_seed(key)

mapping เปลี่ยนเมื่อ seed เปลี่ยน.

ข้อดี:
- collision patterns precompute ยากขึ้น
- adversarial predictability ลดลง

ข้อเสีย:
- reproducibility
- runtime state
- persistence contract ซับซ้อนขึ้น

อย่า persist runtime hash ถ้า API ไม่รับประกัน stability.

## Universal Hashing Preview

เลือก function แบบสุ่มจาก family ที่มี collision probability bound.

แนวคิดนี้ให้ theoretical protection ต่อ arbitrary fixed key sets ดีกว่า fixed deterministic mapping บางแบบ.

## Hash Combining

Composite keys เช่น (user_id,timestamp) ต้อง combine อย่างระวัง.

อย่า XOR fields แบบไม่คิดเรื่อง symmetry/order.
