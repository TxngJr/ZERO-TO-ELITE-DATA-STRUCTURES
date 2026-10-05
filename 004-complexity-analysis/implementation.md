# Implementation — Operation Counting

ไฟล์ examples/operation_counts.c สร้าง counter 4 แบบ:

- constant access
- linear scan
- triangular nested loop
- repeated halving

สำหรับ n ที่ double:
- constant count คงเดิม
- linear count double
- triangular count เข้าใกล้ 4 เท่า
- halving count เพิ่มประมาณ 1

Counter เป็น abstraction ทางการเรียน ไม่ได้บอกจำนวน CPU instructions จริง

benchmarks/scaling.c จึงแยก wall-clock experiment ออกมาต่างหาก เพื่อให้เห็นว่า timing มี noise และ hardware/compiler effects
