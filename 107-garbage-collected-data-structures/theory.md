# Theory — Tracing Garbage Collection

Tracing GC นิยาม live object จาก **reachability from roots**.

ขั้นพื้นฐาน:
1. identify roots
2. trace references
3. mark reachable objects
4. reclaim unmarked objects

ข้อได้เปรียบเหนือ pure reference counting คือ unreachable cycles ถูกเก็บได้.

## Roots

Roots อาจเป็น:
- global references
- stack references
- registers
- runtime handles

Teaching implementation ใช้ explicit root flags เพื่อทำ semantics ให้ deterministic.

## Handles and Moving Collectors

บทนี้ไม่ move objects แต่ใช้ index+generation handle เพื่อสอน stale-reference detection. Moving collector จริงต้อง update references หรือใช้ handle/indirection table อย่างเป็นระบบ.
