# Theory — Persistence

Persistence เป็น property ของ **version history**, ไม่ใช่คำพ้องของ immutability.

Structural sharing ทำให้ cost ต่อ version ต่ำกว่าการ deep-copy ทั้ง structure. ถ้า balanced tree สูง Θ(log n), path copying มักใช้ Θ(log n) new nodes ต่อ update แทน Θ(n).

## Why Sharing Is Safe

การ share ใช้ได้เมื่อ shared node ไม่ถูก mutate. ถ้าเขียนลง shared child หนึ่ง version จะเปลี่ยนอีก version ทันทีและ persistence พัง.

## Memory Reclamation

แนวทางทั่วไป:

- tracing garbage collection
- reference counting
- arena/region lifetime
- epoch/hazard techniques ใน concurrent setting
- custom version GC

บทนี้เลือก arena เพราะ lifetime rule ง่าย: **ทุก node อยู่จน arena ถูกทำลาย**. จึงปลอดภัยต่อ old roots แต่ memory use เพิ่มตามจำนวน successful updates.

## Relation to Functional Structures

Purely functional structures มัก immutable และ persistent โดยธรรมชาติ แต่ persistent structures สามารถ implement ใน imperative language อย่าง C ได้เช่นกันหากควบคุม mutation/lifetime อย่างเข้มงวด.
