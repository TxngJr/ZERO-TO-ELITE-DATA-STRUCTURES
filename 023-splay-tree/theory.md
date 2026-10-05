# Theory — Splay Tree

## Self-adjustment

ไม่มี balance invariant แบบ AVL/RB.

แทนที่จะจ่าย cost เพื่อรักษา strict shape ทุก update, Splay Tree ปรับ shape ตาม access history.

## Why Zig-Zig Is Not Two Random Rotations

Zig-Zig ทำให้ x กระโดดขึ้นสองระดับและลด path structure ในแบบที่สำคัญต่อ amortized proof.

การหมุน x ขึ้นทีละ single Zig อย่างเดียวทุกครั้งไม่ได้ให้ standard splay guarantees เดียวกัน.

## Access Lemma Preview

Splay analysis ใช้ rank/potential จาก subtree sizes เพื่อ bound amortized cost ของ splay.

Course นี้เน้น operational intuition ก่อน; formal potential method revisit Chapter 131.

## Logical Set Preservation

Splaying:
- ไม่เพิ่ม/ลบ key
- รักษา inorder order
- เปลี่ยน root/parent-child topology

ดังนั้น set semantics คงเดิม.
