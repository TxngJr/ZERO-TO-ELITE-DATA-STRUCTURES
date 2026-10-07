# Common Mistakes

- Shuffle feature rows physically every epochโดยไม่จำเป็น.
- Batch order มี duplicate/missing indices.
- อ่าน dataset row ที่ยังไม่ initialize.
- คูณ rows×features โดยไม่เช็ก overflow.
- สับสน embedding lookup กับ nearest-neighbor vector search.
- Gather แล้ว assume output เป็น view ทั้งที่ implementation นี้ copy.
- ใช้ seed ไม่ deterministic แล้ว test reproducibility ยาก.
- คิดว่า contiguous storage แปลว่า every logical batch contiguous.
