# Theory — Fibonacci Heap

## Eager vs Lazy

Binomial Heap eagerly merges equal-degree roots.

Fibonacci Heap postpones this until extract-min.

That trades:
- more roots temporarily
for:
- cheaper insert/meld.

## Why marks exist

Without cascading cuts, repeated decrease-key operations could strip many children from high-degree nodes while leaving degree/subtree-size relations too weak.

Mark rule limits child losses before a node itself is cut.

This enables Fibonacci-like lower bounds on subtree size by degree.

## Amortized viewpoint

A single decrease-key can perform many cuts.

Amortized analysis charges earlier operations for the structural potential represented by roots and marks.

Hence worst actual cost and amortized cost differ.

## Heap-order preservation

Link:
smaller root becomes parent.

Cut:
removing a child cannot violate remaining parent-child heap edges.
Promoted node becomes root, where it has no parent constraint.
