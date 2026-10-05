# Chapter 020 — Balanced BST

## Goal

Chapter 019 แสดงว่า BST ordering invariant อย่างเดียวไม่พอ เพราะ sorted insertion ทำให้ height เป็น Theta(n). บทนี้สอนแนวคิด Balanced BST และ rotation ซึ่งเป็น primitive สำคัญก่อน AVL, Red-Black, Splay และ Treap.

หลังจบบทนี้คุณควร:
- อธิบายว่าทำไม BST ต้องควบคุม height
- แยก ordering invariant กับ balance invariant
- เข้าใจ single left/right rotation
- เข้าใจ double rotations
- พิสูจน์ว่า rotation รักษา inorder order
- เข้าใจ LL/RR/LR/RL patterns
- แยก local restructuring จาก global balancing policy
- เปรียบเทียบ AVL vs Red-Black ในระดับ motivation
- เข้าใจ height metadata และ balance factor

## Why Balance?

Plain BST:

    operations = O(h)

ถ้า h=n-1:
    search/insert/delete = O(n)

Balanced BST family เพิ่ม invariant เพื่อบังคับ h ให้อยู่ระดับ O(log n) หรือมี logarithmic bound.

## Two Invariant Layers

Ordering:

    left keys < node < right keys

Balance:

กำหนด shape เพิ่มเติม เช่น AVL:

    abs(height(left)-height(right)) <= 1

Red-Black ใช้ color/path constraints แทน strict height difference.

## Left Rotation

        x                 y
       / \               / \
      A   y      ->      x   C
         / \            / \
        B   C          A   B

ก่อน rotation:

    A < x < B < y < C

หลัง rotation inequality เหมือนเดิม.

## Right Rotation

Mirror:

          y              x
         / \            / \
        x   C   ->      A   y
       / \                / \
      A   B              B   C

## Parent / Root Updates

Rotation ต้อง update:
- grandparent child link
- new subtree root parent
- pivot parent
- middle subtree parent
- tree root เมื่อหมุนที่ root

## LL / RR / LR / RL

LL:
    right rotation

RR:
    left rotation

LR:
    left rotate child
    then right rotate grandparent

RL:
    right rotate child
    then left rotate grandparent

## Rotation Is Not a Balance Policy

การมี rotate operation ไม่ได้ทำ tree balanced อัตโนมัติ.

ต้องมี policy:
- detect violation
- choose pivot
- choose single/double rotation
- update metadata
- continue upward as required

Chapter 021 AVL ทำ policy นี้จริง.

## Balance Factor

Course convention:

    BF = height(left) - height(right)

AVL valid values:
    -1, 0, +1

## Height Metadata

ถ้าคำนวณ subtree height ใหม่ทั้งต้นไม้ทุก insertion อาจแพง.

Balanced trees จึงมักเก็บ per-node metadata:
- height
- color
- priority
- subtree size

metadata เป็นส่วนของ representation invariant.

## AVL vs Red-Black Preview

AVL:
- stricter balance
- shallow searches
- metadata/rotations on updates

Red-Black:
- looser balance
- logarithmic height bound
- different update trade-offs

ไม่มีอันไหน universally faster.

## Rotation Preservation Properties

Correct rotation preserves:
- number of nodes
- key set
- inorder sequence
- BST ordering

No node allocation/free is needed.

## Complexity

Single rotation:
    Theta(1) pointer rewiring

Public rotate-by-key in teaching code:
    O(h) find pivot + Theta(1) rotation

Balanced-tree search:
    O(log n) only when balance invariant proves logarithmic height.

## Code Scope

RotBST intentionally:
- inserts like ordinary BST
- exposes rotate-left/right by key
- validates ordering/parents
- measures height

It does not auto-balance.
