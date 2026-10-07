# Visual Model

```text
       root
      /    \
    H01    H23
   /  \   /  \
 H0  H1  H2  H3
 |   |   |   |
 L0  L1  L2  L3
```

Proof for L1 contains:
- H0 (left sibling)
- H23 (right sibling)

Verifier:
```text
h = leaf_hash(L1)
h = parent(H0, h)
h = parent(h, H23)
compare h with root
```
