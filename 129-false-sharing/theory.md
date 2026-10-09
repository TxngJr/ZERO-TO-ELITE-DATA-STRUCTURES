# Theory — False Sharing

CPU cache coherenceทำงานเป็นหน่วย cache line ไม่ใช่ C variable.

ถ้า thread A เขียน wordหนึ่งและ thread B เขียนอีก wordหนึ่งใน lineเดียวกัน:
- logical dataแยกกัน
- แต่ coherence ownershipยังแชร์ lineเดียว
- lineอาจ invalidate / migrateระหว่าง coresบ่อย

นี่คือ false sharing.

True sharingต่างกัน: threadsเข้าถึง logical memory locationเดียวกันจริง.

Paddingหรือแยก countersคนละ lineช่วย false sharingได้ แต่เพิ่ม footprint. จึงเป็น time-space trade-off ไม่ใช่ ruleว่า "paddingทุกอย่าง".
