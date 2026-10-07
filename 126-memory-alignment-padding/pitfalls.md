# Common Mistakes

- assume field orderไม่กระทบ sizeof.
- นับเฉพาะ paddingกลาง fieldsแต่ลืม tail padding.
- align-upโดย `(x+a-1)&~(a-1)` โดยไม่เช็ก overflow.
- รับ alignmentที่ไม่ใช่ power of two.
- ใช้ raw pointerที่ถูกเลื่อนแล้วไป `free` แทน original allocation.
- over-alignทุกอย่างแล้วคิดว่าจะเร็วขึ้นเสมอ.
- สับสน memory layoutกับ serialization wire layout.
