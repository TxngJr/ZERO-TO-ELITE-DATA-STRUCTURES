# Implementation Notes

Paths เก็บเป็น nibble bytes 0..15. Fixed 64-nibble keys ทำให้ Branch nodeไม่ต้องมี terminal value.

Insertion เป็น recursive split:
- Leaf vs new key: หา common prefix แล้วสร้าง Branch และ optional Extension
- Extension: ถ้า path match ทั้งหมด recurse child; ถ้าแตกกลาง pathให้ split เป็น Branch
- Branch: consume one nibbleแล้ว recurse child

Node hashesคำนวณ recursivelyจาก canonical bytes. Hashไม่ได้ cache เพื่อให้ validator/root always deriveจาก current structure.

No deletion in this chapter; deletion + branch re-compression is left as an exercise.
