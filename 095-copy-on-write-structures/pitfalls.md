# Common Mistakes

- refs ลดเป็น 0 แต่ไม่ free → backing leak.
- mutate ก่อน detach → clone อื่นเปลี่ยนตาม.
- ลด refcount ก่อน allocation สำเร็จ → failure corrupts ownership.
- คิดว่า COW = immutable/persistent.
- คิดว่า non-atomic refcount thread-safe.
- benchmark เฉพาะ clone แต่ไม่วัด first-write spike.
