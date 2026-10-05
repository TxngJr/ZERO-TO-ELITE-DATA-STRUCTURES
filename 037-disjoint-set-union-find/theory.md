# Theory — DSU

## Equivalence relation

Connectivity represented by DSU behaves like an equivalence relation:
- reflexive
- symmetric
- transitive

The components are equivalence classes.

## Union-by-size depth argument

Whenever a root becomes child of another root under union-by-size, the size of its new containing tree at least doubles.

An element can experience at most log2(n) such root-depth increases.

Therefore without compression:

    height = O(log n)

Path compression improves sequences further to inverse-Ackermann amortized cost.

## Path compression preserves partition

Changing a node parent from an ancestor to the root does not move it to another component because the root is already the representative of that same tree.
