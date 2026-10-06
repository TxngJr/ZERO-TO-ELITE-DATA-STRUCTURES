# Common Mistakes

- ใช้ volatile แทน atomic.
- load แล้ว store แยกเพื่อ implement increment → lost update.
- ใช้ weak CAS ครั้งเดียวแล้วคิดว่า failure = contention จริงเสมอ.
- ใช้ relaxed ordering สำหรับ publication โดยไม่มี proof.
- มอง raw value เท่ากันแล้วคิดว่าไม่มี ABA.
- tag มี version แต่ไม่คิดเรื่อง wrap.
- CAS whole final bitset word แล้วเปิด padding bits.
- atomic primitive ถูกต้องแต่ invariant ของหลาย fields ยัง race.
