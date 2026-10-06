# Chapter 092 — Persistent Data Structures

## Chapter Overview

Persistent Data Structure คือ structure ที่ **version เก่ายังคง query ได้หลัง update ใหม่**. บทนี้ยกระดับจาก Chapter 046 Persistent Segment Tree ซึ่งแสดง persistence ในโครงสร้างเฉพาะ มาเป็น design family: version identity, structural sharing, path copying, lifetime และ reclamation.

Implementation ของบทคือ `PersistentIntSet` แบบ ordered BST ที่ node เป็น immutable หลังสร้าง. ทุก insert/erase สร้าง node ใหม่เฉพาะเส้นทางที่เปลี่ยน และ share subtree ที่ไม่เปลี่ยน. Version handle คือ root pointer.

Memory ถูกจัดการด้วย append-only arena: ทุก version อยู่ได้จน `pset_arena_free`. Design นี้ตั้งใจให้ lifetime model ชัดเจนและหลีกเลี่ยง use-after-free ระหว่าง shared versions; ข้อแลกเปลี่ยนคือไม่ reclaim node ราย version.

## Learning Objectives

หลังจบบท ผู้เรียนต้องสามารถ:

1. อธิบาย ephemeral vs persistent data structure
2. แยก partial persistence, full persistence และ confluent persistence
3. อธิบาย structural sharing และ path copying ด้วย memory diagram
4. สร้าง version ใหม่โดยไม่ mutate version เดิม
5. พิสูจน์ว่า update BST ด้วย path copying รักษา ordering invariant
6. วิเคราะห์จำนวน nodes ใหม่ต่อ update เป็น O(h)
7. อธิบาย arena lifetime และเหตุผลที่ version pointers ยัง valid
8. ทดสอบ historical versions หลัง updates จำนวนมาก
9. แยก persistence ออกจาก immutability
10. อธิบาย trade-off memory reclamation, reference counting, GC, arenas และ epochs

## Prerequisites

- Chapter 019 Binary Search Tree
- Chapter 046 Persistent Segment Tree
- Chapter 092 นี้ต่อโดยตรงจาก Chapter 091 แต่ concept หลักพึ่ง tree/memory มากกว่า succinctness
- Chapter 002 Memory Fundamentals
- Chapter 138 จะ formalize invariants ลึกขึ้นภายหลัง

## Mental Model

Ephemeral tree:

```text
v0:      10
        /  \
       5   20

insert 7 by mutation
         |
         v
v0 ถูกเปลี่ยนไปด้วย
```

Persistent path copying:

```text
v0 root ---> [10]------>[20]
              |
             [5]

insert 7

v1 root ---> [10']----->(share [20])
              |
             [5']-----> [7]

v0 ยังชี้ [10] -> [5] และไม่มี 7
```

เฉพาะ nodes บน search/update path ถูก clone; subtree อื่น share ได้เพราะ node เก่าไม่ถูกแก้.

## Persistence Taxonomy

- **Ephemeral**: update แล้ว state เก่าหาย
- **Partial persistent**: query version เก่าได้ แต่ update เฉพาะ latest
- **Full persistent**: update version ไหนก็ได้และแตก branch ใหม่ได้
- **Confluent persistent**: รวม versions หลาย branch กลับเข้าด้วยกันได้

API บทนี้รับ root ใด ๆ เป็น input update จึงรองรับ branching แบบ full persistence ในเชิง operation model แต่ไม่ได้มี version graph manager ให้โดยอัตโนมัติ.

## Abstract Data Type

Version ของ set รองรับ:

```text
contains(version, key)
insert(version, key) -> new_version
erase(version, key)  -> new_version
size(version)
```

ถ้า insert key ที่มีอยู่ หรือ erase key ที่ไม่มีอยู่ จะคืน root เดิมและไม่ allocate node เพิ่ม.

## Memory Representation

```text
PSetArena
  |
  +--> block --> block --> ...
         |
         +-- PSetNode[256]

PSetNode
+ key
+ subtree size
+ const left
+ const right
```

Node pointer เสถียรเพราะ block ที่ allocate แล้วไม่ realloc/move. Arena มี transaction mark ภายใน update: ถ้า allocation ล้มเหลว nodes ที่ยังไม่ publish จะ rollback.

## Insert

เดินเหมือน BST ปกติจนพบตำแหน่งใหม่ แล้ว unwind:

```text
old child unchanged? share
changed child? allocate copy(parent, new_child, old_other_child)
```

ดังนั้นถ้าความสูงคือ `h`, insert ที่เปลี่ยน set allocate ประมาณ `h+1` nodes.

## Erase

- leaf: version ใหม่ชี้ `NULL` ตรงตำแหน่งนั้น
- one child: ชี้ child เก่าโดย share
- two children: หา inorder successor, path-copy การลบ successor แล้วสร้าง node replacement ใหม่

Version เดิมไม่ถูก mutate.

## Correctness

### Ordering invariant

ทุก node ต้องมี:

```text
left keys < node.key < right keys
```

Path copying ไม่เปลี่ยน key order ของ subtree ที่ share. Node ใหม่ถูกสร้างด้วย child ที่มาจาก recursive operation ซึ่งรับประกัน ordering ภายในช่วงเดิม.

### Persistence invariant

หลัง operation:

```text
all nodes reachable from old root have identical field values as before
```

Code บังคับโดยใช้ `const PSetNode *` links และไม่มี mutation API ของ node.

## Complexity

ถ้า BST height = `h`:

- contains: Θ(h)
- successful insert/erase: Θ(h) time และ O(h) new nodes
- no-op insert/erase: Θ(h) time และ O(1) new nodes
- old-version query: เหมือน current version

เพราะ tree นี้ **ไม่ self-balance**, worst-case `h = n`. Randomized benchmark ใช้กระจาย keys เพื่อลด accidental chain แต่ไม่เปลี่ยน worst-case guarantee.

## Real-World Uses

- undo/redo and editor history
- compiler intermediate representations
- snapshot/config state
- MVCC-inspired versioned indexes
- functional-language collections
- speculative algorithms
- branchable simulations

## When NOT to Use This Design

- latest state เท่านั้นและ memory สำคัญมาก
- updates สร้าง version จำนวนมากแต่ไม่มี reclamation plan
- adversarial ordered keys ทำให้ plain BST เสื่อมเป็น chain
- ต้องการ concurrent reclamation แบบ production-grade
- ต้อง merge versions เข้าด้วยกัน

## Build → Run → Observe → Explain

```bash
cmake -S . -B build
cmake --build build --target ch092_persistent_tests ch092_persistent_benchmark
ctest --test-dir build -R ch092 --output-on-failure
./build/ch092_persistent_benchmark
```

ดู `arena_nodes` เทียบกับ `final_size`: ตัวเลขมากกว่า final nodes เพราะ historical paths ยังคงมีชีวิตเพื่อรองรับ versions.
