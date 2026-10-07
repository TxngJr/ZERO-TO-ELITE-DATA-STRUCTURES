# Common Mistakes

- ใช้ cosine similarity กับ zero vector.
- sort ทุก N distances ทั้งที่ต้องการแค่ top-k.
- max/min heap orientation สลับแล้วทิ้ง candidate ที่ดีที่สุด.
- ไม่กำหนด tie-break ทำให้ test nondeterministic.
- ใช้ Euclidean sqrt ทั้งที่ ranking ด้วย squared distance เหมือนกัน.
- realloc arrays ทีละตัวโดยไม่มี rollback/overflow plan.
- เรียก exact flat index ว่า ANN.
