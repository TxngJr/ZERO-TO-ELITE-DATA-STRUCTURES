# Common Mistakes

- ใช้ reference count แล้วคิดว่า cycle ถูก collect.
- recursion mark graph ลึกมากจน stack overflow.
- allocate mark stack ระหว่าง GC แล้ว collection fail กลางทาง.
- reuse slot โดยไม่เปลี่ยน generation.
- root count ไม่ sync กับ root flags.
- sweep object ที่ยัง reachable.
- live object เก็บ stale child handle.
- เรียก exact collector ว่า conservative GC.
