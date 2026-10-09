# Theory — Locality of Reference

Spatial localityเกิดเมื่อโปรแกรมเข้าถึง address ใกล้กันในช่วงเวลาสั้น ๆ. Cache lineจึงนำข้อมูลรอบ addressที่ร้องขอเข้ามาด้วย.

Temporal localityเกิดเมื่อ addressหรือ cache lineเดิมถูกใช้อีกครั้ง.

Big-O ไม่อธิบาย locality. สอง algorithmsที่เป็น Θ(N) เหมือนกันอาจสร้าง memory tracesต่างกันมาก.

Trace analysisในบทนี้ deliberatelyไม่จำลอง cache capacity/replacement policy. นั่นเป็นหน้าที่ Chapter 128. Chapter 127วัดรูปทรงของ access sequenceอย่างเดียว.
