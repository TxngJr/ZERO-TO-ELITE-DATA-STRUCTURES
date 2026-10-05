# Chapter 017 — Binary Tree

## Goal

Chapter 016 ปูคณิตศาสตร์และศัพท์ของ tree. บทนี้สร้าง mutable pointer-based Binary Tree จริงใน C เพื่อเรียน ownership, parent/child invariants, subtree lifetime และ shape-dependent cost.

หลังจบบทนี้คุณควร:
- implement node ที่มี parent/left/right
- สร้าง root/children
- แยก structural tree จาก BST ordering
- ตรวจ ownership ว่า node อยู่ใน tree ไหน
- remove subtree อย่างปลอดภัย
- รักษา size
- คำนวณ height
- validate parent-child consistency
- เข้าใจ sparse pointer representation
- เปรียบเทียบ pointer tree กับ array tree

## 1. Node Representation

    Node
    +--------+
    | value  |
    | parent |
    | left   |
    | right  |
    +--------+

Tree object:

    +------+
    | root |
    | size |
    +------+

Tree owns every node reachable from root.

## 2. Parent Pointer

parent pointer ช่วย:
- climb upward
- validate membership
- detach subtree
- later BST successor/predecessor designs

Cost:
- one extra pointer per node
- another invariant to maintain

## 3. Ownership

Public API returns borrowed node handles.

Caller:
- may inspect through accessors
- must not free node
- must not use node after its subtree is removed/tree destroyed

Tree:
- owns allocations
- frees nodes exactly once

## 4. Root Creation

Empty tree:

    root=NULL
    size=0

set_root(value):
- allowed only if empty
- allocate node
- parent=NULL
- size=1

## 5. Add Child

add_left(tree,parent,value):
1. verify parent belongs to tree
2. require parent->left==NULL
3. allocate child
4. child->parent=parent
5. parent->left=child
6. size++

Right side analogous.

## 6. Why Membership Matters

Passing a node from another tree must not mutate current tree.

This implementation checks membership by following parent pointers toward tree root with a step bound.

A production API may use encapsulation/generation IDs/arena ownership instead.

## 7. Remove Subtree

To remove node x:
1. verify x belongs to tree
2. detach x from parent or clear root
3. recursively free x and descendants
4. subtract removed node count

All borrowed node pointers inside that subtree become invalid.

## 8. Structural Invariants

- root parent is NULL
- child->parent points back to owner node
- node cannot be both left and right child simultaneously
- every reachable node counted once
- reachable count=size
- no structural cycle
- all nodes reachable from root

## 9. Height

Recursive definition:

    height(leaf)=0
    height(node)=1+max(height(left),height(right))

Empty tree has no size_t height result in this API; function returns false.

Recursive implementation uses O(h) call stack.

## 10. Sparse Shape

Pointer representation stores only existing nodes.

For sparse tree this avoids array holes.

But:
- nodes may be scattered in heap
- pointer chasing
- allocation metadata
- weaker locality

## 11. Array Representation Contrast

Complete tree can use array:
left/right positions computed from index.

Pointer tree:
- arbitrary sparse shape easy
- mutation/linking flexible

Heap chose array because complete shape invariant makes it ideal.
General binary tree does not guarantee that shape.

## 12. Binary Tree Is Not BST

This tree accepts:

        100
       /   \
     999   -20

No ordering rule.

Chapter 019 BST will add:

    left keys < node < right keys

under a chosen duplicate policy.

## 13. Complexity

Let h=height, s=subtree size.

add child:
- membership check O(h)
- allocation/link O(1)

access root/child/value:
- O(1)

height:
- Theta(n)

remove subtree:
- O(h+s) including membership climb + freeing subtree

validate:
- Theta(n) for valid tree

free whole tree:
- Theta(n)

## 14. Recursive Destruction Risk

Recursive free depth is O(h).

A highly skewed giant tree may overflow call stack.

Chapter 018 shows iterative traversal patterns; production destruction can similarly use explicit worklists.

## Files

- src/int_binary_tree.*
- tests/test_binary_tree.c
- examples/binary_tree_demo.c
- benchmarks/tree_shape_benchmark.c
- theory/visual/implementation/complexity/invariants/pitfalls
- lab/exercises/quiz/references
