# Theory — Skip List

## Geometric level distribution

With promotion probability p:

    P(level >= k) = p^(k-1)

Expected nodes at level k:

    n * p^(k-1)

For p=1/2 this halves approximately each level.

## Expected search intuition

Search alternates:
- horizontal progress
- downward movement

Geometric sparsity limits expected horizontal work per level while active level count is O(log n).

Formal probabilistic analysis belongs with Chapter 132 Randomized Structures.

## Invariant vs randomness

Sorted ordering is deterministic invariant.

Random level distribution is not an invariant; it is the assumption behind expected-complexity analysis.

A tree/list can be structurally valid even if unlucky randomness creates poor performance.
