# Invariants

1. Strict BST order: `left < key < right`.
2. `size = 1 + size(left) + size(right)`.
3. Nodes ที่ publish แล้วไม่ถูกแก้ fields.
4. Version root ทุกตัวชี้เฉพาะ nodes ใน arena เดียวที่ยังมีชีวิต.
5. No-op update คืน root เดิม.
6. Allocation failure ไม่ publish partial version.
7. Shared subtree มี semantics เดิมในทุก version ที่อ้างถึง.

Historical-version randomized tests เป็น invariant test ที่สำคัญพอ ๆ กับ validator ของ current tree.
