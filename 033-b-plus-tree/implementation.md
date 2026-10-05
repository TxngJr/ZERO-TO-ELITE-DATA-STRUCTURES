# Implementation — IntBPlusTree

Node:
- leaf flag
- key_count
- keys array
- children array for internal nodes
- next pointer for leaves

Tree:
- minimum degree t
- root
- size

Temporary capacities allow one overflow entry during recursive insert.

Internal separator keys are always recomputed as minimum keys of children 1..m.
