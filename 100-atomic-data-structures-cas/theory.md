# Theory — Atomic Operations and CAS

Atomic operation คือ operation ที่ language memory model ให้ indivisible semantics ตาม primitive นั้น แต่ไม่ได้หมายความว่า sequence ของ atomics หลายตัวรวมกัน atomic โดยอัตโนมัติ.

## Read / Write / RMW

- load: อ่าน atomic object
- store: เขียน atomic object
- read-modify-write: อ่านค่าเก่า + คำนวณ + publish ค่าใหม่แบบ atomic
- CAS: เขียน desired เฉพาะเมื่อ current ตรง expected

## Strong vs Weak CAS

Strong CAS ไม่ควร fail เมื่อค่าเท่ากับ expected.
Weak CAS อาจ fail แบบ spurious และจึงเหมาะกับ retry loop.

## Memory Orders

บทนี้ใช้ acquire, release และ acq_rel เป็นหลัก. Relaxed ใช้กับ metrics ที่ไม่ publish object state.

## ABA

Equality ของ raw value ไม่ได้พิสูจน์ว่า state “ไม่เคยเปลี่ยน”. Version tag ช่วย detect A→B→A แต่ tag เองมี finite width และอาจ wrap ในระบบอายุยาวมาก.
