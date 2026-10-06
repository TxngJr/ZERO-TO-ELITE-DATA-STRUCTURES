# Common Mistakes

- ใช้ `if` แทน `while` รอบ condition wait.
- Signal ก่อน state mutation หรือ mutate นอก mutex.
- Close queue แล้วทิ้ง elements ที่ยังค้างโดยไม่กำหนด contract.
- Readers ของ map ไม่ lock เพราะ “อ่านอย่างเดียว”.
- Rehash โดยไม่ล็อกทุก representation ที่เกี่ยวข้อง.
- ทำ compound check-then-act จากหลาย API calls แล้วคิดว่า atomic.
- Free queue/map ขณะ worker ยังใช้.
