# Common Mistakes

- ใช้ benchmark timeเป็น correctness test.
- นับ unique elementsแทน unique cache lines.
- คำนวณ `index * element_size` โดยไม่เช็ก overflow.
- ใช้ sentinel line ID 0 ทั้งที่ address 0 valid.
- เรียก temporal reuseว่า cache hit; hitจริงขึ้นกับ capacity/associativity/replacement.
- สร้าง modular strideที่ไม่ coprimeกับ countแล้วคิดว่าเป็น permutation.
- สรุป performanceจาก VM runเดียว.
