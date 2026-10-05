# Chapter 021 — AVL Tree

## Goal

AVL Tree คือ self-balancing BST ที่รักษาความต่างของ subtree heights ทุก node ไม่เกิน 1. บทนี้นำ rotation primitive จาก Chapter 020 มารวมกับ stored height metadata และ rebalance policy จริง.

หลังจบบทนี้คุณควร:
- นิยาม AVL invariant
- เก็บ/update node height
- คำนวณ balance factor
- detect LL/RR/LR/RL
- rebalance insertion
- rebalance deletion
- พิสูจน์ BST ordering + AVL balance
- เข้าใจ logarithmic height
- ทดสอบ randomized insert/remove
- เปรียบเทียบ plain BST กับ AVL บน sorted insertion

## Height Convention

Internal metadata ใช้:

    height(NULL)=0
    height(leaf)=1

Public API แปลงเป็น edge-height:

    public_height = root.height - 1

การเลือก node-height metadata ทำให้ formula update ง่าย:

    height(node)=1+max(height(left),height(right))

## Balance Factor

Course convention:

    BF = height(left)-height(right)

AVL invariant:

    -1 <= BF <= 1

สำหรับทุก node.

## Insert

1. BST insert recursively.
2. unwind recursion.
3. update height.
4. compute BF.
5. rotate if BF outside [-1,1].

Because only ancestors of inserted leaf can change height, rebalancing follows insertion path.

## LL

BF(node)>1 and key inserted into left-left direction.

Right rotation.

## RR

BF(node)<-1 and insertion in right-right.

Left rotation.

## LR

left-heavy but left child is right-heavy:

1. node->left = rotate_left(node->left)
2. rotate_right(node)

## RL

right-heavy but right child is left-heavy:

1. node->right = rotate_right(node->right)
2. rotate_left(node)

## Delete

Deletion starts as BST deletion.

After child/subtree removal:
- update height
- rebalance on every ancestor during recursion unwind

Deletion differs from insertion because subtree height can decrease and several ancestors may need rebalancing.

Cases are selected from child balance factors, not deleted key direction alone.

## Two-Child Delete

This implementation copies inorder-successor key into the node, then recursively removes the successor from right subtree.

Public API intentionally does not expose node handles, so key-copying does not violate a node-identity contract.

## Why Height Is O(log n)

AVL balance yields a minimum-node recurrence for height h:

    N(h) = 1 + N(h-1) + N(h-2)

with Fibonacci-like growth.

Thus N(h) grows exponentially in h, so:

    h = O(log n)

This is the key guarantee plain BST lacked.

## Search

AVL search is ordinary BST search.

Because h=O(log n):

    search = O(log n)

## Insert / Delete

They follow root-to-key path plus constant-time metadata/rotations per visited ancestor:

    O(log n)

Rotations themselves are Theta(1).

## Metadata Invariant

For each node:

    stored_height
      = 1 + max(stored_height(left), stored_height(right))

A tree can have correct ordering but stale height metadata and therefore be invalid AVL representation.

Validator must check both.

## Duplicate Policy

Unique integer keys.
Duplicate insert returns false and does not mutate tree.

## Node Identity

API exposes keys/operations, not node pointers.

This simplifies:
- deletion
- rotations
- successor replacement
- lifetime safety

Later chapters can discuss stable-node-handle APIs separately.

## AVL vs Plain BST

Sorted insertion 1..n:

Plain BST:
    height=n-1

AVL:
    height=Theta(log n)

This is benchmarked directly.

## AVL vs Red-Black Preview

AVL usually maintains tighter height.
Red-Black uses weaker balancing with color rules.

Chapter 022 implements Red-Black Tree and compares the invariants directly.

## Files

- src/int_avl.*
- tests/test_avl.c
- examples/avl_demo.c
- benchmarks/avl_vs_bst.c
- theory/visual-model/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
