# Common Mistakes

- ใช้ modulo head/tail อย่างเดียวกับ MPMC แล้วเกิด generation ambiguity.
- Publish sequence ก่อนเขียน payload.
- Reuse slot ก่อน consumer release.
- คิดว่า atomic positions ทำให้ payload access safe โดยอัตโนมัติ.
- เรียก algorithm lock-free เพียงเพราะไม่มี mutex.
- ใช้ concurrent enqueue-dequeue difference เป็น exact size โดยไม่พิสูจน์.
- Ignore counter wrap semantics.
- ใช้ capacity ที่ไม่เป็น power of twoแต่ยังใช้ bit mask.
