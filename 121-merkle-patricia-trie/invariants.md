# Invariants

1. Every path nibble is in 0..15.
2. Leaf path consumes all remaining key nibbles.
3. Extension path length > 0.
4. Extension child is non-null.
5. Branch has at least two non-null children in canonical compressed form.
6. No Extension points directly to another Extension after insertion normalization.
7. A key follows exactly one root-to-leaf path.
8. Leaf value is bound into the leaf hash.
9. Root hash derives only from canonical node content.
10. Equivalent key/value sets produce the same root regardless of insertion order.
