# Theory

Thread-safe API ต้องระบุ:
1. operation ไหน atomic ในเชิง object semantics
2. operation ไหน block
3. shutdown/close semantics
4. lifetime ownership
5. compound actions ที่ยังต้อง external transaction

ตัวอย่าง `if (!map_get(k)) map_put(k,v)` เป็นสอง linearizable calls แต่ทั้งคู่รวมกันยังไม่ใช่ atomic check-and-insert.

Condition variables เป็น waiting mechanism ไม่ใช่ state. Predicate เช่น `size < capacity` หรือ `size > 0` ต้องถูกตรวจภายใต้ mutex และตรวจซ้ำหลัง wake.

Lock striping ลด contention แต่เพิ่ม complexity เรื่อง multi-bucket operations และ lock ordering.
