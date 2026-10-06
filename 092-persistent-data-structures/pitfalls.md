# Common Mistakes

## Mutate shared child
**Symptom:** old version เปลี่ยนตาม new version.
**Fix:** node ที่ publish แล้วต้อง immutable; update ด้วย path copying.

## `realloc` node arena
**Symptom:** historical pointers dangling.
**Fix:** ใช้ stable-address blocks.

## Deep-copy ทั้ง tree
Correct แต่เสีย Θ(n) memory/time ต่อ updateและพลาดจุดสำคัญของ structural sharing.

## ลืม reclamation model
Arena ง่ายแต่ memory โตตาม history. Production system ต้องกำหนดว่า version ไหนยัง live.

## อ้าง O(log n) โดยไม่ balance
Plain BST worst-case Θ(n). ต้องรายงาน `h` ก่อนแล้วค่อย derive guarantee.

## Allocation failure ระหว่าง path copy
ถ้าปล่อย unpublished nodesค้าง อาจทำ memory growth แบบเงียบ; implementation นี้ใช้ arena mark/rollback.
