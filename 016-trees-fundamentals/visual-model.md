# Visual Model — Trees

## Terminology

        A
      /   \
     B     C
    / \     \
   D   E     F

root=A
leaves=D,E,F
internal=A,B,C
height=2 under edge-height convention.

## Full but not perfect

        A
       / \
      B   C
         / \
        D   E

Full เพราะ internal nodes มีสอง children.
Not perfect เพราะ leaves ต่าง depth.

## Complete but not perfect

        A
       / \
      B   C
     / \
    D   E

last level filled from left.

## Skewed

A
 \
  B
   \
    C
     \
      D

n=4, height=3.
