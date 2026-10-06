# Common Mistakes

- ใช้ `volatile` แทน atomics/mutex.
- Lock writes แต่ปล่อย readers อ่านระหว่าง realloc.
- ทำ size atomic แต่ปล่อย array non-atomic แล้วเรียกทั้ง structure thread-safe.
- `if(!contains(x)) insert(x)` อาจมี higher-level race เพราะสอง calls ไม่ใช่ transaction เดียว.
- free object ขณะ workers ยังใช้.
- เรียก mutex design ว่า lock-free.
- คิดว่า TSan ผ่าน = proof of linearizability/deadlock freedom.
