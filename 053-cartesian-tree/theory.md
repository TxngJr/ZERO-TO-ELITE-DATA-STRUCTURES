# Theory — Cartesian Tree

## Uniqueness

If all values are distinct, the Cartesian Tree is uniquely determined by:
- inorder sequence order
- min-heap property

With duplicates, a tie policy is needed.

This implementation keeps earlier equal indices above later equal indices.

## Recursive definition

For interval [l,r):
1. find the leftmost minimum index m
2. m is subtree root
3. left child is Cartesian Tree of [l,m)
4. right child is Cartesian Tree of [m+1,r)

Naively finding every minimum can cost Theta(n^2).

The monotonic-stack algorithm constructs the same tree in Theta(n).

## RMQ/LCA equivalence

For indices i<=j, the minimum on [i,j] corresponds to the lowest common ancestor of nodes i and j in the min Cartesian Tree.

This is a major bridge between:
- arrays
- trees
- Euler tours
- static RMQ
