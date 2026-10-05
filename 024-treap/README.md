# Chapter 024 — Treap

## Goal

Treap = Tree + Heap.

Treap รักษา invariant สองชุดพร้อมกัน:
- BST order ตาม key
- Heap order ตาม random priority

เมื่อ priorities สุ่มอย่างอิสระ shape ของ Treap มีพฤติกรรมเหมือน random BST และให้ expected logarithmic height.

หลังจบบทนี้คุณควร:
- เข้าใจ Cartesian combination ของ key order + priority order
- implement insertion + rotations
- implement deletion via merge
- แยก deterministic invariant จาก probabilistic complexity
- เข้าใจ randomization assumptions
- handle equal priorities ด้วย deterministic tie-break
- validate BST + heap + parent links
- randomized differential test
- compare Treap กับ AVL/RB/Splay

## Two Orders at Once

BST invariant:

    left.key < node.key < right.key

Heap invariant ในบทนี้ใช้ min-heap priority.

เพื่อจัดการ priority ซ้ำ เรากำหนด total heap order:

    (priority, key)

คู่ที่เล็กกว่า lexicographically มี heap priority สูงกว่า.

ดังนั้น parent ต้องไม่ด้อยกว่า child ตาม pair order.

## Why Tie-break Matters

ถ้า priorities เท่ากันแล้วไม่มี policy:
- rotation decision อาจกำกวม
- validator contract ไม่ชัด
- deterministic tests ยาก

Unique keys ทำให้ pair (priority,key) เป็น total order.

## Random Priorities

Default insert สร้าง uint32 priority จาก PRNG ภายใน tree.

Theoretical expected O(log n) ต้องอาศัยสมมติฐานว่าลำดับ priorities behave like independent random ranks เพียงพอ.

PRNG teaching implementation:
- deterministic seed เพื่อ reproducible tests
- ไม่ใช่ cryptographic RNG
- ไม่เหมาะใช้สร้าง secrets

## Insert

1. BST insert by key.
2. assign priority.
3. while node outranks parent:
   - if node is left child: rotate right(parent)
   - if right child: rotate left(parent)
4. stop at root or heap invariant restored.

Ordering preserved by rotations.

## Explicit-Priority Insert

API มี:

    insert_with_priority(key, priority)

ใช้สำหรับ:
- diagrams
- deterministic tests
- teaching rotations

Default insert uses PRNG.

## Delete via Merge

เมื่อเจอ node ที่ต้องลบ:
- left subtree keys < node.key
- right subtree keys > node.key

ลบ node แล้ว merge(left,right).

Merge เลือก root ที่มี heap pair สูงกว่า:
- ถ้า left root wins:
    left.right = merge(left.right,right)
- ถ้า right root wins:
    right.left = merge(left,right.left)

เพราะทุก key ฝั่ง left < ทุก key ฝั่ง right, BST order ยังคงถูกต้อง.

Heap choice ทำให้ priority invariant ถูกต้องด้วย.

## Split Preview

Treap ยังรองรับ elegant split operation:

    split(tree,key) -> (keys < key, keys >= key)

เมื่อมี split + merge เราสามารถสร้าง operations หลายแบบได้ง่าย.

Course implementation รอบนี้ใช้ rotations สำหรับ insert และ merge สำหรับ delete เพื่อให้เห็นทั้งสองแนว.

## Expected Complexity

Random-priority Treap:

Expected height:
    O(log n)

Expected:
- search O(log n)
- insert O(log n)
- delete O(log n)

Worst-case:
    O(n)

เพราะ randomization ไม่ใช่ deterministic balance guarantee.

## Treap vs AVL / Red-Black

AVL:
- deterministic strict height balance
- stored height

Red-Black:
- deterministic color invariants
- logarithmic worst-case height

Treap:
- simpler randomized balancing
- stores priority
- expected logarithmic shape
- worst-case linear possible

## Treap vs Splay

Splay:
- no random priority
- adapts to access history
- amortized guarantee

Treap:
- shape driven by priorities
- ordinary search does not mutate tree
- expected guarantee from randomization

## Priority Is Representation Metadata

User's logical set is keys.

Random priority:
- not part of set identity
- controls shape

Changing priorities arbitrarily without rotations can invalidate representation.

## Complexity Table

| Operation | Expected | Worst |
|---|---:|---:|
| contains | O(log n) | O(n) |
| insert | O(log n) | O(n) |
| remove | O(log n) | O(n) |
| inorder | Theta(n) | Theta(n) |
| validate | Theta(n) | Theta(n) |

## Files

- src/int_treap.*
- tests/test_treap.c
- examples/treap_demo.c
- benchmarks/treap_vs_bst.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
