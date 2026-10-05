# Chapter 016 — Trees Fundamentals

## Goal

Tree เป็นโครงสร้างแบบ hierarchical ที่ต่างจาก linear structures ตรงที่ node หนึ่งสามารถนำไปสู่หลาย descendants ได้ บทนี้ปู vocabulary, mathematical properties, recursive definitions, shape, height/depth และ invariants ก่อนลงมือสร้าง Binary Tree ใน Chapter 017.

หลังจบบทนี้คุณควร:
- แยก root, parent, child, sibling, ancestor, descendant
- แยก leaf/internal node
- เข้าใจ edge, path, path length
- แยก depth, height และ level โดยระบุ convention
- เข้าใจ subtree
- แยก ordered/unordered และ rooted/unrooted tree
- เข้าใจ degree/branching factor
- อธิบาย binary, full, perfect, complete, balanced และ skewed tree
- derive จำนวน nodes ของ perfect binary tree
- เข้าใจ recursive tree definition
- แยก tree จาก graph ที่มี cycle/multiple-parent
- เห็นผลของ shape ต่อ complexity

## Rooted Tree

ตัวอย่าง:

    A
   / \
  B   C
 / \
D   E

A=root, B/C=children of A, D/E=siblings.

ทุก node ยกเว้น root มี parent เดียว และมี simple path เดียวจาก root ถึง node นั้น.

## Depth and Height

Course convention:

    depth(root)=0
    height(leaf)=0

height วัดจำนวน edges บนเส้นทางลงลึกที่สุด.

empty tree ใช้ height=-1 ในเชิงคณิตศาสตร์เมื่อสูตรต้องการ แต่ C API ในบทนี้ไม่แทน -1 ด้วย size_t.

## Degree

degree(node)=จำนวน children.

Binary tree:
    degree <= 2

## Leaf / Internal Node

Leaf ไม่มี children.
Internal node มี child อย่างน้อยหนึ่ง.

Single-node tree มี root ที่เป็น leaf ด้วย.

## Subtree

Subtree rooted at x คือ x และ descendants ทั้งหมด.

นิยามนี้ทำให้ recursive algorithms เป็นธรรมชาติ.

## Ordered Tree

Binary tree ปกติเป็น ordered tree:
left และ right มีบทบาทต่างกัน.

## Binary Tree

แต่ละ node มี at most 2 children.

Binary Tree ไม่เท่ากับ Binary Search Tree.
BST เพิ่ม ordering invariant ซึ่งจะเรียน Chapter 019.

## Full Binary Tree

ทุก node มี 0 หรือ 2 children.

## Perfect Binary Tree

ทุก internal node มี 2 children และ leaves อยู่ depth เดียวกัน.

height h:

    nodes = 1+2+4+...+2^h
          = 2^(h+1)-1

    leaves = 2^h

## Complete Binary Tree

ทุก level ยกเว้นสุดท้ายเต็ม และ level สุดท้ายเติมจากซ้ายไปขวา.

Binary Heap จาก Chapter 012 ใช้ shape นี้.

Complete ไม่เท่ากับ Perfect.

## Balanced Tree

คำว่า balanced ต้องมี formal definition.

โดยกว้าง ๆ หมายถึงควบคุม height ไม่ให้โตเป็น Theta(n), แต่ AVL, Red-Black และ balance schemes อื่นมี invariant ต่างกัน.

อย่าแทน h ด้วย log n ถ้ายังไม่มี balance guarantee.

## Skewed Tree

    A
     \
      B
       \
        C
         \
          D

n=4, height=3.

## Shape Controls Cost

หลาย tree operations ขึ้นกับ h:

    O(h)

Balanced:
    h=O(log n)

Skewed:
    h=Theta(n)

## Tree vs General Graph

Ordinary rooted tree:
- ไม่มี cycle
- non-root node มี parent เดียว
- ทุก node reachable จาก root
- unique root-to-node path

DAG ที่ node มีหลาย parents ไม่ใช่ tree ตาม model นี้.

## Recursive Definition

Binary tree:

    Tree = Empty
         | Node(value, left: Tree, right: Tree)

นี่เป็นฐานของ structural induction และ recursive traversal.

## Representations

- pointer-based nodes
- array-based complete-tree layout
- parent array
- first-child/next-sibling สำหรับ arbitrary-degree trees

## Metrics

size = total nodes
height = max root-to-leaf edge distance
width(level) = nodes at that depth
maximum width = max across levels

## Empty vs Single Node

Empty:
    size=0, no root

Single node:
    size=1, height=0

Code ในบทนี้เป็น tree-math utilities เพื่อฝึก properties; mutable Binary Tree เริ่ม Chapter 017.
